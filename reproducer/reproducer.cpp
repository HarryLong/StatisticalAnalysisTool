#include "reproducer.h"
#include "../utils/file_utils.h"
#include "../utils/utils.h"
#include "distribution_factory.h"
#include "../analysis_point.h"

#include <math.h>
#include <iostream>
#include <chrono>
#include <QStringList>
#include <thread>
#include <QDebug>

StrengthCalculationTracker::StrengthCalculationTracker(int count) : _n_complete(0), _strength(1), _n_total(count)
{

}

StrengthCalculationTracker::~StrengthCalculationTracker()
{

}

void StrengthCalculationTracker::complete(float strength) const
{
    _strength *= strength;
    ++_n_complete;
}

bool StrengthCalculationTracker::finished() const
{
    return _n_complete == _n_total;
}

float StrengthCalculationTracker::strength() const
{
    return _strength;
}

//-----------------------------------------------------------

StrengthCalculationSpec::StrengthCalculationSpec(const StrengthCalculationTracker & tracker, QLineF line, int r_min, int r_diff, int r_max,
                                                 const RadialDistribution & radial_distribution, const AnalysisPoint & destination_point) :
    tracker(tracker), line(line), r_min(r_min), r_max(r_max), radial_distribution(radial_distribution), destination_point(destination_point)
{

}

StrengthCalculationSpec::~StrengthCalculationSpec()
{

}

void StrengthCalculationSpec::calculate()
{
    float strength(1.f);
    // Get the pair correlation

    line.setP2(destination_point.getCenter());
    float distance(line.length()-destination_point.getRadius()); // Distance from the reference points circumference [remove the radius]

    // Check if its within the radius
    if(distance < .0f) // within radius
        strength *= radial_distribution.m_within_radius_distribution;
    else
    {
        int r_bracket ( RadialDistributionUtils::getRBracket(distance, r_min, r_diff) );
        if(r_bracket < r_max)
            strength *= radial_distribution.m_data.find(r_bracket)->second;
    }

    tracker.complete(strength);
}

//-----------------------------------------------------------

RadialDistributionReproducer::GeneratedPoints RadialDistributionReproducer::reproduce(ReproductionConfiguration reproduction_configuration,
                                                                                      GeneratedPointsProperties * outGeneratedPointProperties)
{
    if(!FileUtils::check_directory_structure(reproduction_configuration.active_directory))
    {
        std::cerr << "Incorrect directory structure in " << reproduction_configuration.active_directory.toStdString() << ". Unable to reproduce..." << std::endl;
        exit(1);
    }

    AnalysisConfiguration analysis_configuration(FileUtils::get_configuration_file(reproduction_configuration.active_directory).toStdString());

    // Load category properties
    CategoryPropertiesContainer loaded_category_properties;
    {
        std::vector<QString> category_properties_files(FileUtils::get_category_properties_files(reproduction_configuration.active_directory));
        for(QString category_property_file : category_properties_files)
        {
            CategoryProperties category_property(category_property_file.toStdString());
            loaded_category_properties.insert(std::pair<int, CategoryProperties>(category_property.m_header.category_id, category_property));
        }

        // Check all necessary category properties files were there
        for(auto it(analysis_configuration.priority_sorted_category_ids.begin()); it != analysis_configuration.priority_sorted_category_ids.end(); it++)
        {
            if(loaded_category_properties.find(*it) == loaded_category_properties.end())
            {
                std::cerr << "Missing category properties file for category " << *it << std::endl;
                exit(1);
            }
        }
    }

    std::vector<std::pair<int,int> > required_pair_correlations;
    for(auto reference_category(analysis_configuration.priority_sorted_category_ids.begin()); reference_category != analysis_configuration.priority_sorted_category_ids.end(); reference_category++)
    {
        for(auto target_category(std::find(analysis_configuration.priority_sorted_category_ids.begin(), analysis_configuration.priority_sorted_category_ids.end(), *reference_category));
                            target_category != analysis_configuration.priority_sorted_category_ids.end(); target_category++)
        {
            required_pair_correlations.push_back(std::pair<int,int>(*reference_category, *target_category));
        }
    }

    // Load radial distributions
    PairCorrelations loaded_pair_correlations;
    {
        std::vector<QString> radial_distribution_files(FileUtils::get_radial_distribution_files(reproduction_configuration.active_directory));
        for(QString radial_distribution_file : radial_distribution_files)
        {
            RadialDistribution radial_distribution(radial_distribution_file.toStdString());
            radial_distribution.printToConsole();
            std::pair<int,int> pair(radial_distribution.m_header.reference_id, radial_distribution.m_header.destination_id);
            loaded_pair_correlations.insert(std::pair<std::pair<int,int>, RadialDistribution>(pair, radial_distribution));
        }

        // Check all necessary pair correlations were loaded
        for(auto it(required_pair_correlations.begin()); it != required_pair_correlations.end(); it++)
        {
            if(loaded_pair_correlations.find(*it) == loaded_pair_correlations.end())
            {
                std::cerr << "Missing pair correlation for category pair [" << it->first << "," << it->second << "]" << std::endl;
                exit(1);
            }
        }
    }

    // We're good to go
    RadialDistributionReproducer reproducer(loaded_pair_correlations, loaded_category_properties, reproduction_configuration,
                                            analysis_configuration, outGeneratedPointProperties);

    reproducer.startPointGeneration();

    return reproducer.getGeneratedPoints();
}


#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
/////////////////////////////////////////////////////////////////////////////////
RadialDistributionReproducer::RadialDistributionReproducer(PairCorrelations pair_correlations, CategoryPropertiesContainer category_properties,
                                                           ReproductionConfiguration reproduction_settings, AnalysisConfiguration analysis_configuration,
                                                           GeneratedPointsProperties * outGeneratedPointProperties) :
    m_pair_correlations(pair_correlations), m_category_properties(category_properties), m_reproduction_configuration(reproduction_settings),
    m_spatial_point_storage(m_reproduction_configuration.width, m_reproduction_configuration.height), m_analysis_configuration(analysis_configuration),
    m_point_factory(reproduction_settings.width, reproduction_settings.height),
    m_generated_points_properties(outGeneratedPointProperties)
{
    // Initialize the point properties tracker
    SizeProperties size_properties;
    for(auto it(category_properties.begin()); it != category_properties.end(); it++)
    {
        size_properties.heightToCanopyRadius = it->second.m_header.height_to_radius_multiplier;
        size_properties.minHeight = -1;
        size_properties.maxHeight = -1;
        m_generated_points_properties->emplace(it->first, size_properties);
    }
}

RadialDistributionReproducer::~RadialDistributionReproducer()
{

}

void RadialDistributionReproducer::startPointGeneration()
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point end_time;

    int total_point_count(0);

    for(auto it(m_analysis_configuration.priority_sorted_category_ids.begin()); it != m_analysis_configuration.priority_sorted_category_ids.end(); it++)
    {
        int id(*it);
        std::cout << "Inserting points for category " << id << "..." << std::endl;

        m_point_factory.setCategoryProperties(m_category_properties.find(id)->second);

        generate_points();

        if(m_active_category_points.size() > 0)
        {
            m_all_generated_points[m_active_category_points.at(0).getCategoryId()] = m_active_category_points;
            total_point_count += m_active_category_points.size();
        }

        m_active_category_points.clear();
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto duration (std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count());

    std::cout << "Simulation over! " << std::endl
//              << "\t Source density: " << (((float)m_radial_distribution.m_properties.n_reference_points)/m_radial_distribution.m_properties.analysed_area) << std::endl
//              << "\t Output density: " << ((float)m_destination_points->size())/(m_settings.width*m_settings.height) << std::endl
              << "\t Calculation time: " << duration << " ms." << std::endl
              << "\t Total points: " << total_point_count << std::endl;
}

void RadialDistributionReproducer::generate_points()
{
    matching_density_initialize();
    generate_points_through_random_moves();
}

bool RadialDistributionReproducer::requires_optimization(int category_id)
{
    bool requires_optmization(false);
    for(auto it(m_pair_correlations.begin()); it != m_pair_correlations.end() && !requires_optmization; it++)
    {
        if(it->first.first == category_id || it->first.second == category_id)
            requires_optmization = it->second.m_header.requires_optimization;
    }

    return requires_optmization;
}

void RadialDistributionReproducer::generate_points_through_random_moves()
{
    DiceRoller dice_roller(0,1000);

    int n_accepted_moves(0), n_refused_moves(0);
    int points_processed(0);
    for(int selected_point_index(0); selected_point_index < m_active_category_points.size(); selected_point_index++)
    {
        AnalysisPoint & selected_point(m_active_category_points.at(selected_point_index));

        double source_point_strength( calculate_strength(selected_point) );

        QPoint heighest_scoring_dest_point;
        double heighest_strength(-1);

        for(int i(0); i < 2; i++)
        {
            /*********************
             * DESTINATION POINT *
             *********************/
            AnalysisPoint random_dest_point = AnalysisPoint( m_point_factory.getPoint(selected_point.getHeight()) );

            // Calculate strength
            double destination_point_strength = calculate_strength(random_dest_point);
//            std::cout << "Random destination point strength: " << destination_point_strength << std::endl;

            if(destination_point_strength > heighest_strength)
            {
                heighest_scoring_dest_point = random_dest_point.getCenter();
                heighest_strength = destination_point_strength;
            }
        }

        double acceptance_ratio(heighest_strength/source_point_strength);
//        std::cout << "Acceptance ratio: " << acceptance_ratio << std::endl;
        if(ProbabilisticUtils::returnTrueWithProbability(acceptance_ratio, dice_roller))
        {
//            std::cout << "ACCEPTED" << std::endl;
            n_accepted_moves++;
            move_point(selected_point, heighest_scoring_dest_point);
        }
        else
            n_refused_moves++;

        if((points_processed++)%1000 == 0)
            std::cout << ((((float)points_processed)/m_active_category_points.size()) * 100) << "%" << std::endl;
    }

    std::cout << "Accepted moves: " << n_accepted_moves << " | Refused moves: " << n_refused_moves << std::endl;
}

void RadialDistributionReproducer::generate_points_through_births_and_deaths()
{
//    DiceRoller dice_roller(0,1000);
//    int n_births(0), n_deaths(0);

//    for(int i(0); i < m_reproduction_configuration.n_iterations; i++)
//    {
//        if(ProbabilisticUtils::returnTrueWithProbability(.5f, dice_roller)) // Birth
//        {
//            AnalysisPoint * random_point(m_point_factory.getPoint());

//            float point_strength(calculate_strength(random_point));

//            if(ProbabilisticUtils::returnTrueWithProbability(point_strength, dice_roller)) // Insert the point!
//            {
//                n_births++;
//                add_destination_point(random_point);
//            }
//            else
//                delete random_point;
//        }
//        else // Death
//        {
//            if(m_active_category_points.size() > 2) // Don't attempt to kill if less than 3 elements
//            {
//                // Get a random point from the generated points
//                int selected_point_index (rand()%m_active_category_points.size());
//                AnalysisPoint* selected_point( m_active_category_points.at(selected_point_index) );

//                // Calculate the strength
//                float point_strength(calculate_strength(selected_point));

//                if(!ProbabilisticUtils::returnTrueWithProbability(point_strength, dice_roller)) // Remove the point
//                {
//                    n_deaths++;
//                    remove_destination_point(selected_point, selected_point_index);
//                }
//            }
//        }
//        if(i%10000 == 0)
//            std::cout << ((((float)i)/m_reproduction_configuration.n_iterations) * 100) << "%" << std::endl;
//    }

//    std::cout << "Births: " << n_births << " | Deaths: " << n_deaths << std::endl;
}

void RadialDistributionReproducer::two_point_initialize()
{
//    // POINT 1
//    AnalysisPoint * p1(m_point_factory.getPoint());
//    add_destination_point(p1);

//    // POINT 2
//    AnalysisPoint * p2(m_point_factory.getPoint());
//    while(calculate_strength(p2) == 0)
//    {
//        delete p2;
//        p2 = m_point_factory.getPoint();
//    }

//    add_destination_point(p2);
}

int RadialDistributionReproducer::matching_density_initialize()
{
    int active_category_id(m_point_factory.getActiveCategoryId());

    int points_in_analysis(m_category_properties.find(active_category_id)->second.m_header.n_points);

    int n_points(points_in_analysis * (((float)m_reproduction_configuration.width*m_reproduction_configuration.height) /
                                        (m_analysis_configuration.analysis_window_width*m_analysis_configuration.analysis_window_height)));

    int i(0);
    bool valid, strength_calculation_necessary;

    while(m_active_category_points.size() < n_points)
    {
        AnalysisPoint random_point(m_point_factory.getPoint());

        // Attempt accelerated point validity check
        accelerated_point_validity_check(random_point, valid, strength_calculation_necessary);

//        if(calculate_strength(random_point) > 0)
        if((!strength_calculation_necessary && valid) || (strength_calculation_necessary && calculate_strength(random_point) > 0))
        {
            add_destination_point(random_point);
        }

        if(i++ % 10000 == 0)
        {
            std::cout << "Points generated: " << m_active_category_points.size() << " / " << n_points << std::endl;
        }
    }

    return n_points;
}

void RadialDistributionReproducer::move_point(AnalysisPoint & point, const QPoint & new_location)
{
    // Remove
    m_point_factory.setPositionStatus(point.getCenter(), true); // Update the point factory
    m_spatial_point_storage.removePoint(point);

    point.setCenter(new_location);

    // Add
    m_point_factory.setPositionStatus(new_location, false); // Update the point factory
    m_spatial_point_storage.addPoint(point);
}

void RadialDistributionReproducer::add_destination_point(const AnalysisPoint & point)
{
    m_point_factory.setPositionStatus(point.getCenter(), false); // Update the point factory
    m_active_category_points.push_back(point); // Update the active category points vector
    m_spatial_point_storage.addPoint(point);

    // Check min/max
    SizeProperties & props((*m_generated_points_properties)[point.getCategoryId()]);
    int point_height(point.getHeight());
    if(props.maxHeight == -1 || point.getHeight() > props.maxHeight)
        props.maxHeight = point_height;
    if(props.minHeight == -1 || point.getHeight() < props.minHeight)
        props.minHeight = point_height;
}

void RadialDistributionReproducer::remove_destination_point(const AnalysisPoint & point, int this_category_points_index)
{
    m_point_factory.setPositionStatus(point.getCenter(), true); // Update the point factory
    m_active_category_points.erase(m_active_category_points.begin()+this_category_points_index); // Update the active category points vector

    m_spatial_point_storage.removePoint(point);
}

void RadialDistributionReproducer::accelerated_point_validity_check(const AnalysisPoint & reference_point, bool & valid, bool & strength_calculation_necessary)
{
    // Check pair correlations for this point and other points present and check if any are equal to zero
    for(auto it(m_all_generated_points.begin()); it != m_all_generated_points.end(); it++)
    {
        accelerated_point_validity_check(reference_point, it->first, valid, strength_calculation_necessary);

        if(strength_calculation_necessary || (!strength_calculation_necessary && !valid))
            return;
    }

    // With itself
    accelerated_point_validity_check(reference_point, m_point_factory.getActiveCategoryId(), valid, strength_calculation_necessary);
}

void RadialDistributionReproducer::accelerated_point_validity_check(const AnalysisPoint & reference_point, int queried_category, bool & valid,
                                                                    bool & strength_calculation_necessary)
{
    // Start negative
    valid = false;
    strength_calculation_necessary = true;


    if(m_category_properties.find(reference_point.getCategoryId())->second.m_header.category_dependent_ids.size() > 0) // dependent categories
        return;

    const RadialDistribution distribution (get_radial_distribution(queried_category, reference_point.getCategoryId()));
    if(distribution.getMinimum() == 0)
    {
        if(distribution.m_past_rmax_distribution == 0 ||
                distribution.m_within_radius_distribution == 0 ||
                    m_category_properties.find(reference_point.getCategoryId())->second.m_header.category_dependent_ids.size() > 0)
            return;

        for(auto it(distribution.m_data.begin()); it != distribution.m_data.end(); it++)
        {
            if(it->second == 0)
                return;
        }

//        if(distribution.m_within_radius_distribution == 0) // most probable, just attempt to find points
//        {
//            QLineF line;
//            line.setP1(reference_point.getCenter());
//            std::vector<AnalysisPoint> possible_reachable_points = m_spatial_point_storage.getPossibleReachablePoints(reference_point,0);
//            for(AnalysisPoint & p : possible_reachable_points)
//            {
//                if(p.getCategoryId() == queried_category)
//                {
//                    line.setP2(p.getCenter());
//                    float distance(line.length()-p.getRadius()); // Distance from the reference points circumference [remove the radius]

//                    if(distance < 0) // within radius
//                    {
//                        valid = false;
//                        strength_calculation_necessary = false;
//                        return;
//                    }
//                }
//            }
//        }
    }

    // If we got here, point is valid
    valid = true;
    strength_calculation_necessary = false;
}

double RadialDistributionReproducer::calculate_strength(const AnalysisPoint & reference_point)
{
    std::vector<AnalysisPoint> possible_reachable_points(m_spatial_point_storage.getPossibleReachablePoints(reference_point, m_analysis_configuration.r_max));
//    std::cout << "Reachable points: " << possible_reachable_points.size() << std::endl;
    return calculate_strength(reference_point, possible_reachable_points);
}
double RadialDistributionReproducer::calculate_strength(const AnalysisPoint & candidate_point, const std::vector<AnalysisPoint> & existing_points)
{
    double strength(1.0f);

    QLineF line;
    line.setP1(candidate_point.getCenter());

    CategoryProperties category_properties (m_category_properties.find(candidate_point.getCategoryId())->second);
    std::set<int> dependent_category_ids(category_properties.m_header.category_dependent_ids);

    // If it's a depended point, check it it's within the radius
    if(!dependent_category_ids.empty())
    {
        bool within_radius(false);
        for(int i(0); i < existing_points.size() && !within_radius ; i++)
        {
            const AnalysisPoint & existing_point(existing_points.at(i));
            if(std::find(dependent_category_ids.begin(), dependent_category_ids.end(), existing_point.getCategoryId()) != dependent_category_ids.end());
            {
                line.setP2(existing_point.getCenter());
                if(line.length() < existing_point.getRadius())
                {
                    within_radius = true;
                }
            }
        }
        if(!within_radius)
            return 0; // Not within radius of dependent point
    }
    int processed_point_count(0);
    for(const AnalysisPoint & existing_point : existing_points)
    {
        // Get the pair correlation
        const RadialDistribution & radial_distribution( get_radial_distribution(existing_point.getCategoryId(), candidate_point.getCategoryId()) );

        line.setP2(existing_point.getCenter());
        float distance(line.length()-existing_point.getRadius()); // Distance from the reference points circumference [remove the radius]

        // Check if its within the radius
        if(distance < .0f) // within radius
        {
            processed_point_count++;
//            std::cout << "Within radius " << std::endl;
            strength *= radial_distribution.m_within_radius_distribution;
        }
        else
        {
            int r_bracket ( RadialDistributionUtils::getRBracket(distance, m_analysis_configuration.r_min, m_analysis_configuration.r_diff) );
            if(r_bracket < m_analysis_configuration.r_max)
            {
                float r(radial_distribution.m_data.find(r_bracket)->second);
//                std::cout << "Bracket: " << r_bracket << " | R: " << r << std::endl;
                strength *= r;
                processed_point_count++;
            }
        }

        if(strength == 0) // Optimization. It will always be zero
            return strength;
    }

    return strength;
}

RadialDistributionReproducer::GeneratedPoints & RadialDistributionReproducer::getGeneratedPoints()
{
    return m_all_generated_points;
}

const RadialDistribution & RadialDistributionReproducer::get_radial_distribution(int reference_category, int target_category)
{
    auto pair_correlation_it (m_pair_correlations.find(std::pair<int,int>(reference_category, target_category)));
    if( pair_correlation_it == m_pair_correlations.end())
    {
        std::cerr << "FAILED TO FIND PAIR CORRELATION DATA!" << std::endl;
        exit(1);
    }
    return pair_correlation_it->second;
}
