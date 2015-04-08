#include "radial_distribution_reproducer.h"
#include "utils.h"
#include "distribution_factory.h"
#include "analysis_point.h"

#include <math.h>
#include <QLineF>
#include <iostream>

#include <chrono>


#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
/////////////////////////////////////////////////////////////////////////////////
RadialDistributionReproducer::RadialDistributionReproducer(std::vector<RadialDistribution> radial_distributions, std::vector<PointSizeProperties> point_size_properties,
                                                           PriorityToCategoryMapper category_priorities, Settings reproduction_settings) :
    m_settings(reproduction_settings), m_generated_points_spatial_hashmap(SPATIAL_HASHMAP_CELL_WIDTH, SPATIAL_HASHMAP_CELL_HEIGHT,
                                                                          std::ceil(((float)m_settings.width)/SPATIAL_HASHMAP_CELL_WIDTH),
                                                                          std::ceil(((float)m_settings.height)/SPATIAL_HASHMAP_CELL_HEIGHT)),
    m_priority_to_category_ids(category_priorities), m_r_min(-1), m_r_max(-1), m_r_diff(-1),
    m_point_factory(reproduction_settings.width, reproduction_settings.height)
{
    // Insert the radial distributions
    for(RadialDistribution radial_distribution : radial_distributions)
    {
        m_pair_correlations.insert(std::pair<std::pair<int,int>, RadialDistribution>(
                                    std::pair<int,int>(radial_distribution.m_properties.reference_id, radial_distribution.m_properties.destination_id), radial_distribution) );
        m_pair_correlations.insert(std::pair<std::pair<int,int>, RadialDistribution>(
                                    std::pair<int,int>(radial_distribution.m_properties.destination_id, radial_distribution.m_properties.reference_id), radial_distribution) );

        if(m_category_densities.find(radial_distribution.m_properties.reference_id) == m_category_densities.end())
            m_category_densities.insert(std::pair<int,float>(radial_distribution.m_properties.reference_id,
                                                         ((float)radial_distribution.m_properties.n_reference_points)/ radial_distribution.m_properties.analysed_area));

        if(m_r_min == -1)
        {
            m_r_min = radial_distribution.m_properties.r_min;
            m_r_max = radial_distribution.m_properties.r_max;
            m_r_diff = radial_distribution.m_properties.r_diff;
        }
        else if(m_r_min != radial_distribution.m_properties.r_min ||
                m_r_max != radial_distribution.m_properties.r_max ||
                m_r_diff != radial_distribution.m_properties.r_diff)
        {
            std::cerr << "In order to reproduce a radial distribution, all radial distributions must share the same r_min, r_max and r_diff configuration!" << std::endl;
            exit(1);
        }
    }
    // Insert the category size properties
    for(PointSizeProperties point_size_property : point_size_properties)
    {
        m_category_size_properties.insert(std::pair<int, PointSizeProperties>(point_size_property.m_header.points_id, point_size_property));
    }
}

RadialDistributionReproducer::~RadialDistributionReproducer()
{

}

void RadialDistributionReproducer::startPointGeneration()
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point end_time;

    for(auto it(m_priority_to_category_ids.rbegin()); it != m_priority_to_category_ids.rend(); it++)
    {
        int id(it->second);
        m_point_factory.setPointSizeProperties(m_category_size_properties.find(id)->second);
        generate_points();

        m_all_generated_points.insert(m_all_generated_points.end(), m_this_category_points.begin(), m_this_category_points.end());
        m_this_category_points.clear();
    }

    end_time = std::chrono::high_resolution_clock::now();

    auto duration (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());

    std::cout << "Simulation over! " << std::endl
//              << "\t Source density: " << (((float)m_radial_distribution.m_properties.n_reference_points)/m_radial_distribution.m_properties.analysed_area) << std::endl
//              << "\t Output density: " << ((float)m_destination_points->size())/(m_settings.width*m_settings.height) << std::endl
              << "\t Calculation time: " << duration << " ms." << std::endl
              << "\t Total points: " << m_all_generated_points.size() << std::endl;
}

void RadialDistributionReproducer::generate_points()
{
    switch (m_settings.initialization_type) {
    case Settings::InitializationType::TwoPoints:
        two_point_initialize();
        break;
    case Settings::InitializationType::MatchDensity:
        matching_density_initialize();
        break;
    }

    switch(m_settings.generation_algo){
    case Settings::GenerationAlgorithm::BirthsAndDeaths:
        generate_points_through_births_and_deaths();
        break;
    case Settings::GenerationAlgorithm::RandomMoves:
        generate_points_through_random_moves();
        break;
    }

}

void RadialDistributionReproducer::generate_points_through_random_moves()
{
    DiceRoller dice_roller(0,1000);

    int n_accepted_moves(0), n_refused_moves(0);

    for(int i(0); i < m_settings.n_iterations; i++)
    {
        float source_point_strength(.0f), destination_point_strength(.0f);

        /****************
         * CHOSEN POINT *
         ****************/
        // Select a source point at random
        int selected_point_index (rand()%m_this_category_points.size());
        AnalysisPoint* selected_point( m_this_category_points.at(selected_point_index) );

        // Calculate source point strength
        source_point_strength = calculateStrength(selected_point);


        /*********************
         * DESTINATION POINT *
         *********************/
        AnalysisPoint * destination_point( m_point_factory.getPoint() );

        // Calculate strength
        destination_point_strength = calculateStrength(destination_point);

        float acceptance_ratio(destination_point_strength/source_point_strength);
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

        if(i%10000 == 0)
            std::cout << ((((float)i)/m_settings.n_iterations) * 100) << "%" << std::endl;
    }
    std::cout << "Accepted moves: " << n_accepted_moves << " | Refused moves: " << n_refused_moves << std::endl;
}

void RadialDistributionReproducer::generate_points_through_births_and_deaths()
{
    DiceRoller dice_roller(0,1000);
    int n_births(0), n_deaths(0);

    for(int i(0); i < m_settings.n_iterations; i++)
    {
        if(ProbabilisticUtils::returnTrueWithProbability(.5f, dice_roller)) // Birth
        {
            AnalysisPoint * random_point(m_point_factory.getPoint());

            float point_strength(calculateStrength(random_point));

            if(ProbabilisticUtils::returnTrueWithProbability(point_strength, dice_roller)) // Insert the point!
            {
                n_births++;
                add_destination_point(random_point);
            }
            else
                delete random_point;
        }
        else // Death
        {
            if(m_this_category_points.size() > 2) // Don't attempt to kill if less than 3 elements
            {
                // Get a random point from the generated points
                int selected_point_index (rand()%m_this_category_points.size());
                AnalysisPoint* selected_point( m_this_category_points.at(selected_point_index) );

                // Calculate the strength
                float point_strength(calculateStrength(selected_point));

                if(!ProbabilisticUtils::returnTrueWithProbability(point_strength, dice_roller)) // Remove the point
                {
                    n_deaths++;
                    remove_destination_point(selected_point, selected_point_index);
                }
            }
        }
        if(i%10000 == 0)
            std::cout << ((((float)i)/m_settings.n_iterations) * 100) << "%" << std::endl;
    }

    std::cout << "Births: " << n_births << " | Deaths: " << n_deaths << std::endl;
}

void RadialDistributionReproducer::two_point_initialize()
{
    // POINT 1
    AnalysisPoint * p1(m_point_factory.getPoint());
    add_destination_point(p1);

    // POINT 2
    AnalysisPoint * p2(m_point_factory.getPoint());
    while(calculateStrength(p2) == 0)
    {
        delete p2;
        p2 = m_point_factory.getPoint();
    }

    add_destination_point(p2);
}

void RadialDistributionReproducer::matching_density_initialize()
{
    int n_points(m_category_densities[m_point_factory.getActiveCategoryId()] * m_settings.width*m_settings.height);

    int i(0);

    while(m_this_category_points.size() < n_points)
    {
        AnalysisPoint * random_point(m_point_factory.getPoint());
        if(calculateStrength(random_point) > 0)
        {
            add_destination_point(random_point);
        }
        else
            delete random_point;

        if(i++ % 10000 == 0)
        {
            std::cout << "Points generated: " << m_this_category_points.size() << " / " << n_points << std::endl;
        }
    }
}

void RadialDistributionReproducer::add_destination_point(AnalysisPoint *point)
{
    m_point_factory.setPositionStatus(point->getCenter(), false); // Update the point factory
    m_this_category_points.push_back(point); // Update the active category points vector
    m_generated_points_spatial_hashmap.getCell(point->getCenter(),true)->points.push_back(point); // Update the spatial hashmap
}

void RadialDistributionReproducer::remove_destination_point(AnalysisPoint * point, int this_category_points_index)
{
    m_point_factory.setPositionStatus(point->getCenter(), true); // Update the point factory
    m_this_category_points.erase(m_this_category_points.begin()+this_category_points_index); // Update the active category points vector

    SpatialHashMapCell* cell (m_generated_points_spatial_hashmap.getCell(point->getCenter())); // Update the spatial hashmap
    cell->points.erase(std::find(cell->points.begin(), cell->points.end(), point));

    delete point;
}

float RadialDistributionReproducer::calculateStrength(const AnalysisPoint* reference_point)
{
    float strength(1.0f);

    std::vector<SpatialHashMapCell*> cells_within_r_max(m_generated_points_spatial_hashmap.getCells(reference_point->getCenter(),
                                                                                       m_r_max+m_generated_points_spatial_hashmap.getCellWidth()+reference_point->getRadius()));

    for(SpatialHashMapCell* cell : cells_within_r_max)
    {
        if(cell)
            strength *= calculateStrength(reference_point, cell->points);
    }

    return strength;
}

float RadialDistributionReproducer::calculateStrength(const AnalysisPoint* reference_point, const std::vector<AnalysisPoint*> & destination_points)
{
    float strength(1.0f);

    int normalization_length(reference_point->getRadius()-1); // Perform analysis for each point normalized to a length of 1
    QLineF line;
    line.setP1(reference_point->getCenter());
    for(AnalysisPoint* destination_point : destination_points)
    {
        if(reference_point != destination_point)
        {
            line.setP2(destination_point->getCenter());
            float length(line.length()-normalization_length);

            int r_bracket ( RadialDistributionUtils::getRBracket(length, m_r_min, m_r_diff) );
            if(r_bracket < m_r_max)
            {
                RadialDistribution & radial_distribution( m_pair_correlations.find(std::pair<int,int>(reference_point->getCategoryId(), destination_point->getCategoryId()))->second );
                strength *= radial_distribution.m_data.find(r_bracket)->second;
            }
        }
        if(strength == 0) // Optimization. It will always be zero
            return strength;
    }

    return strength;
}

std::vector<AnalysisPoint*> & RadialDistributionReproducer::getGeneratedPoints()
{
    return m_all_generated_points;
}
