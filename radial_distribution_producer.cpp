#include "radial_distribution_producer.h"
#include "utils.h"
#include "distribution_factory.h"
#include "spatial_hashmap.h"

#include <math.h>
#include <QLineF>
#include <iostream>

#include <chrono>

PointMap::PointMap()
{

}

void PointMap::insertPoint(QPoint p)
{
    insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(p.x(),p.y()), true));
}

void PointMap::removePoint(QPoint p)
{
    erase(std::pair<int,int>(p.x(),p.y()));
}

bool PointMap::containsPoint(QPoint p)
{
    return find(std::pair<int,int>(p.x(),p.y())) != std::map<std::pair<int,int>,int>::end();
}

std::map<std::pair<int,int>,int>::iterator PointMap::begin()
{
    return std::map<std::pair<int,int>,int>::begin();
}

std::map<std::pair<int,int>,int>::iterator PointMap::end()
{
    return std::map<std::pair<int,int>,int>::end();
}

/////////////////////////////////////////////////////////////////////////////////

RadialDistributionProducer::RadialDistributionProducer(std::string radial_distribution_file):
    m_radial_distribution(radial_distribution_file)
{

}

RadialDistributionProducer::RadialDistributionProducer(RadialDistribution distribution) :
    m_radial_distribution(distribution)
{

}

RadialDistributionProducer::~RadialDistributionProducer()
{

}

#define PROCESSING_FILE "/home/harry/radial_distributions/processing/processing_"
#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
std::vector<QPoint> RadialDistributionProducer::generatePoints(int width, int height, int iterations, bool verbose)
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point end_time;

    float target_density(((float)m_radial_distribution.m_properties.n_reference_points)/m_radial_distribution.m_properties.analysed_area);

    // To speed the search of points within r_max, points are stored in a spatial_hashmap
    SpatialHashMap<SpatialHashMapCell> spatial_point_storage(SPATIAL_HASHMAP_CELL_WIDTH, SPATIAL_HASHMAP_CELL_HEIGHT,
                                                          std::ceil(((float)width)/SPATIAL_HASHMAP_CELL_WIDTH),
                                                          std::ceil(((float)height)/SPATIAL_HASHMAP_CELL_HEIGHT));

    // First generate two points at random
    std::vector<QPoint> raw_points (initialize(width, height));
    for(QPoint p : raw_points)
        spatial_point_storage.getCell(p, true)->points.push_back(p);
    std::map<std::pair<int,int>,int> existing_point_coordinates;

    DiceRoller dice_roller(0,1000);
    int births(0), deaths(0);

//    float current_density;
//    int i(0);
//    while((current_density = ((float)raw_points.size())/(width*height)) < target_density)
    for(int i(0); i < iterations; i++)
    {
//        float source_point_strength, destination_point_strength;

//        /****************
//         * SOURCE POINT *
//         ****************/
//        // Select a source point at random
//        int source_point_index (rand()%raw_points.size());
//        QPoint source_point( raw_points.at(source_point_index) );

//        // Calculate source point strength
//        {
//            std::vector<SpatialHashMapCell*> cells_within_r_max(spatial_point_storage.getCells(source_point, m_radial_distribution.m_properties.r_max));
//            source_point_strength = calculateStrength(source_point, cells_within_r_max);
//            source_point_strength *= source_point_strength;
//        }

//        /*********************
//         * DESTINATION POINT *
//         *********************/
//        //Random destination for point
//        QPoint destination_point;
//        do{
//            destination_point = get_random_point(width, height);
//        }while(existing_point_coordinates.find(std::pair<int,int>(destination_point.x(), destination_point.y())) != existing_point_coordinates.end());

//        // Calculate strength
//        {
//            std::vector<SpatialHashMapCell*> cells_within_r_max(spatial_point_storage.getCells(destination_point, m_radial_distribution.m_properties.r_max));
//            float destination_point_strength(calculateStrength(destination_point, cells_within_r_max));
//            destination_point_strength *= destination_point_strength;
//        }

//        float acceptance_ratio(destination_point_strength/source_point_strength);
//        if(ProbabilisticUtils::returnTrueWithProbability(acceptance_ratio, dice_roller))
//        {
//            // Remove the source point
//            SpatialHashMapCell* cell (spatial_point_storage.getCell(source_point));
//            cell->points.erase(std::find(cell->points.begin(), cell->points.end(), source_point));
//            // From the raw points
//            raw_points.erase(raw_points.begin()+source_point_index);
//            existing_point_coordinates.erase(std::pair<int,int>(source_point.x(),source_point.y()));

//            // Add the new point
//            spatial_point_storage.getCell(destination_point, true)->points.push_back(destination_point);
//            raw_points.push_back(destination_point);
//            existing_point_coordinates.insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(destination_point.x(), destination_point.y()), 1));
//        }

        if(ProbabilisticUtils::returnTrueWithProbability(.5f, dice_roller)) // Birth
        {
            QPoint random_point;
            do{
                random_point = get_random_point(width, height);
            }while(existing_point_coordinates.find(std::pair<int,int>(random_point.x(), random_point.y())) != existing_point_coordinates.end());

            std::vector<SpatialHashMapCell*> cells_within_r_max(spatial_point_storage.getCells(random_point, m_radial_distribution.m_properties.r_max));

            float point_strength(calculateStrength(random_point, cells_within_r_max));
            point_strength *= point_strength;

            if(ProbabilisticUtils::returnTrueWithProbability(point_strength, dice_roller)) // Insert the point!
            {
                births++;
                spatial_point_storage.getCell(random_point, true)->points.push_back(random_point);
                raw_points.push_back(random_point);
                existing_point_coordinates.insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(random_point.x(), random_point.y()), 1));
            }
        }
        else // Death
        {
            if(raw_points.size() > 2) // Don't attempt to kill if less than 3 elements
            {
                // Get a random point from the generated points
                int selected_point_index (rand()%raw_points.size());
                QPoint selected_point( raw_points.at(selected_point_index) );

                // Calculate the strength
                std::vector<SpatialHashMapCell*> cells_within_r_max(spatial_point_storage.getCells(selected_point, m_radial_distribution.m_properties.r_max));
                float point_strength(calculateStrength(selected_point, cells_within_r_max));
                point_strength *= point_strength;

                if(!ProbabilisticUtils::returnTrueWithProbability(point_strength, dice_roller)) // Remove the point
                {
                    deaths++;
                    // From the spatial hashmap
                    SpatialHashMapCell* cell (spatial_point_storage.getCell(selected_point));
                    cell->points.erase(std::find(cell->points.begin(), cell->points.end(), selected_point));

                    // From the raw points
                    raw_points.erase(raw_points.begin()+selected_point_index);

                    existing_point_coordinates.erase(std::pair<int,int>(selected_point.x(),selected_point.y()));
                }
            }
        }
        if(verbose)
        {
            static int processing_file_index = 0;
            if(i%10000 == 0)
            {
//                std::cout << "Density: " << current_density << " / " << target_density << std::endl;
                std::cout << ((((float)i)/iterations) * 100) << "%" << std::endl;
//                std::string filename(PROCESSING_FILE);
//                filename.append(std::to_string(processing_file_index++));
//                filename.append(".jpg");
//                FileUtils::printPointsToImg(filename, raw_points, width, height);
            }
         }
    }
    end_time = std::chrono::high_resolution_clock::now();

    auto duration (std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count());

    std::cout << "Simulation over! " << std::endl
              << "\t Births: " << births << std::endl
              << "\t Deaths: " << deaths << std::endl
              << "\t Source density: " << (((float)m_radial_distribution.m_properties.n_reference_points)/m_radial_distribution.m_properties.analysed_area) << std::endl
              << "\t Output density: " << ((float)raw_points.size())/(width*height) << std::endl
              << "\t Calculation time: " << duration << " ms." << std::endl
              << "\t Total points: " << raw_points.size() << std::endl;

    return raw_points;
}

std::vector<QPoint> RadialDistributionProducer::initialize(int width, int height)
{
//    int n_points((((float)m_radial_distribution.m_properties.n_reference_points)/m_radial_distribution.m_properties.analysed_area) * (width*height));

//    std::vector<QPoint> generated_points;
//    PointMap generated_point_map;

//    int i(0);

//    while(generated_points.size() < n_points)
//    {
//        QPoint random_point(get_random_point(width,height));
//        if(!generated_point_map.containsPoint(random_point) && calculateStrength(random_point, generated_points) > 0)
//        {
//            generated_points.push_back(random_point);
//            generated_point_map.insertPoint(random_point);
//        }

//        if(i++ % 10000 == 0)
//        {
//            std::cout << "Points generated: " << generated_points.size() << " / " << n_points << std::endl;
//        }
//    }

//    return generated_points;

    QPoint p1(width/2.0f, height/2.0f), p2(width/2.0f, height/2.0f);

    bool point_found(false);
    int p2_x(p1.x());
    while(!point_found && (p2_x += m_radial_distribution.m_properties.r_diff) < width)
    {
        if(m_radial_distribution.m_data[RadialDistributionUtils::getRBracket(p2_x-p1.x(), m_radial_distribution.m_properties.r_min, m_radial_distribution.m_properties.r_diff)] > 0)
        {
            p2.setX(p2_x);
            point_found = true;
        }
    }

    if(!point_found)
    {
        std::cerr << "Unable to initialize points with non-zero strength!" << std::endl;
        exit(1);
    }

    std::vector<QPoint> ret;
    ret.push_back(p1);
    ret.push_back(p2);

    return ret;
}

float RadialDistributionProducer::calculateStrength(const QPoint & reference_point, const std::vector<SpatialHashMapCell*> & destination_points)
{
    float strength(1.0f);
    for(SpatialHashMapCell* cell : destination_points)
    {
        if(cell)
            strength *= calculateStrength(reference_point, cell->points);
    }

    return strength;
}

float RadialDistributionProducer::calculateStrength(const QPoint & reference_point, const std::vector<QPoint> & destination_points)
{
    float strength(1.0f);

    QLineF line;
    line.setP1(reference_point);
    for(QPoint destination_point : destination_points)
    {
        if(reference_point != destination_point)
        {
            line.setP2(destination_point);
            int r_bracket ( RadialDistributionUtils::getRBracket(line.length(), m_radial_distribution.m_properties.r_min, m_radial_distribution.m_properties.r_diff) );
            if(r_bracket < m_radial_distribution.m_properties.r_max)
            {
                strength *= m_radial_distribution.m_data.find(r_bracket)->second;
            }
        }
        if(strength == 0) // Optimization. It will always be zero
            return strength;
    }

    return strength;
}

QPoint RadialDistributionProducer::get_random_point(int max_width, int max_height)
{
   return QPoint(rand() % max_width, rand() % max_height);
}
