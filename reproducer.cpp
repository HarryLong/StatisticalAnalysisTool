#include "reproducer.h"
#include "utils.h"
#include "distribution_factory.h"
#include "analysis_point.h"
#include "analysis_configuration.h"

#include <math.h>
#include <QLineF>
#include <iostream>

#include <chrono>
#include <QStringList>

std::map<int,std::vector<AnalysisPoint*> > RadialDistributionReproducer::reproduce(ReproductionConfiguration reproduction_configuration)
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
                                            analysis_configuration);

    reproducer.startPointGeneration();

    return reproducer.getGeneratedPoints();
}


#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
/////////////////////////////////////////////////////////////////////////////////
RadialDistributionReproducer::RadialDistributionReproducer(PairCorrelations pair_correlations, CategoryPropertiesContainer category_properties,
                                                           ReproductionConfiguration reproduction_settings, AnalysisConfiguration analysis_configuration) :
    m_pair_correlations(pair_correlations), m_category_properties(category_properties), m_reproduction_configuration(reproduction_settings),
    m_spatial_point_storage(m_reproduction_configuration.width, m_reproduction_configuration.height), m_analysis_configuration(analysis_configuration),
    m_point_factory(reproduction_settings.width, reproduction_settings.height)
{

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
            m_all_generated_points[m_active_category_points.at(0)->getCategoryId()] = m_active_category_points;
            total_point_count += m_active_category_points.size();
        }

        m_active_category_points.clear();
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto duration (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());

    std::cout << "Simulation over! " << std::endl
//              << "\t Source density: " << (((float)m_radial_distribution.m_properties.n_reference_points)/m_radial_distribution.m_properties.analysed_area) << std::endl
//              << "\t Output density: " << ((float)m_destination_points->size())/(m_settings.width*m_settings.height) << std::endl
              << "\t Calculation time: " << duration << " ms." << std::endl
              << "\t Total points: " << total_point_count << std::endl;
}

void RadialDistributionReproducer::generate_points()
{
    switch (m_reproduction_configuration.initialization_type) {
    case ReproductionConfiguration::InitializationType::TwoPoints:
        two_point_initialize();
        break;
    case ReproductionConfiguration::InitializationType::MatchDensity:
        matching_density_initialize();
        break;
    }

    switch(m_reproduction_configuration.generation_algo){
    case ReproductionConfiguration::GenerationAlgorithm::BirthsAndDeaths:
        generate_points_through_births_and_deaths();
        break;
    case ReproductionConfiguration::GenerationAlgorithm::RandomMoves:
        generate_points_through_random_moves();
        break;
    }
}

void RadialDistributionReproducer::generate_points_through_random_moves()
{
    DiceRoller dice_roller(0,1000);

    int n_accepted_moves(0), n_refused_moves(0);

    for(int i(0); i < m_reproduction_configuration.n_iterations; i++)
    {
        float source_point_strength(.0f), destination_point_strength(.0f);

        /****************
         * CHOSEN POINT *
         ****************/
        // Select a source point at random
        int selected_point_index (rand()%m_active_category_points.size());
        AnalysisPoint* selected_point( m_active_category_points.at(selected_point_index) );

        // Calculate source point strength
        source_point_strength = calculate_strength(selected_point);

        /*********************
         * DESTINATION POINT *
         *********************/
        AnalysisPoint * destination_point( m_point_factory.getPoint() );

        // Calculate strength
        destination_point_strength = calculate_strength(destination_point);

        float acceptance_ratio(destination_point_strength/source_point_strength);

        //        std::cout << "Source point strength: " << source_point_strength << " | Destination point strength: " << destination_point_strength << std::endl;

        if(ProbabilisticUtils::returnTrueWithProbability(acceptance_ratio, dice_roller))
        {
            n_accepted_moves++;

            // Remove the soruce point
            remove_destination_point(selected_point, selected_point_index);

            // Add the new point
            add_destination_point(destination_point);
        }
        else
        {
            n_refused_moves++;
            delete destination_point;
        }

        if(i%100 == 0)
            std::cout << ((((float)i)/m_reproduction_configuration.n_iterations) * 100) << "%" << std::endl;
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

void RadialDistributionReproducer::matching_density_initialize()
{
    int active_category_id(m_point_factory.getActiveCategoryId());

    int points_in_analysis(m_category_properties.find(active_category_id)->second.m_header.n_points);

    int n_points(points_in_analysis * (((float)m_reproduction_configuration.width*m_reproduction_configuration.height) /
                                        (m_analysis_configuration.analysis_window_width*m_analysis_configuration.analysis_window_height)));

    int i(0);

    while(m_active_category_points.size() < n_points)
    {
        AnalysisPoint * random_point(m_point_factory.getPoint());

        if(calculate_strength(random_point) > 0)
        {
            add_destination_point(random_point);
        }
        else
            delete random_point;

        if(i++ % 10000 == 0)
        {
            std::cout << "Points generated: " << m_active_category_points.size() << " / " << n_points << std::endl;
        }
    }
}

void RadialDistributionReproducer::add_destination_point(AnalysisPoint *point)
{
    m_point_factory.setPositionStatus(point->getCenter(), false); // Update the point factory
    m_active_category_points.push_back(point); // Update the active category points vector
    m_spatial_point_storage.addPoint(point);
}

void RadialDistributionReproducer::remove_destination_point(AnalysisPoint * point, int this_category_points_index)
{
    m_point_factory.setPositionStatus(point->getCenter(), true); // Update the point factory
    m_active_category_points.erase(m_active_category_points.begin()+this_category_points_index); // Update the active category points vector

    m_spatial_point_storage.removePoint(point);

    delete point;
}

float RadialDistributionReproducer::calculate_strength(const AnalysisPoint* reference_point)
{
    std::vector<AnalysisPoint*> possible_reachable_points(m_spatial_point_storage.getPossibleReachablePoints(reference_point, m_analysis_configuration.r_max));

    return calculate_strength(reference_point, possible_reachable_points);
}

float RadialDistributionReproducer::calculate_strength(const AnalysisPoint* candidate_point, const std::vector<AnalysisPoint*> & existing_points)
{
    float strength(1.0f);

    QLineF line;
    line.setP1(candidate_point->getCenter());

    CategoryProperties category_properties (m_category_properties.find(candidate_point->getCategoryId())->second);
    std::set<int> dependent_category_ids(category_properties.m_header.category_dependent_ids);

    bool within_radius_of_dependent_point ( dependent_category_ids.empty() );

    for(AnalysisPoint* existing_point : existing_points)
    {
        // Get the pair correlation
        auto pair_correlation_it (m_pair_correlations.find(std::pair<int,int>(existing_point->getCategoryId(), candidate_point->getCategoryId())));
        if( pair_correlation_it == m_pair_correlations.end())
        {
            std::cerr << "FAILED TO FIND PAIR CORRELATION DATA!" << std::endl;
            exit(1);
        }
        RadialDistribution & radial_distribution( pair_correlation_it->second) ;

        line.setP2(existing_point->getCenter());
        float distance(line.length()-existing_point->getRadius()); // Distance from the reference points circumference [remove the radius]

        // Check if its within the radius
        if(distance < .0f) // within radius
        {
            if(!within_radius_of_dependent_point)
                within_radius_of_dependent_point = (std::find(dependent_category_ids.begin(), dependent_category_ids.end(), existing_point->getCategoryId()) != dependent_category_ids.end());
            strength *= radial_distribution.m_within_radius_distribution;
        }
        else
        {
            int r_bracket ( RadialDistributionUtils::getRBracket(distance, m_analysis_configuration.r_min, m_analysis_configuration.r_diff) );
            if(r_bracket < m_analysis_configuration.r_max)
            {
                strength *= radial_distribution.m_data.find(r_bracket)->second;
            }
        }

        if(strength == 0) // Optimization. It will always be zero
            return strength;
    }

    return (within_radius_of_dependent_point ? strength : 0);
}

std::map<int,std::vector<AnalysisPoint*> >& RadialDistributionReproducer::getGeneratedPoints()
{
    return m_all_generated_points;
}
