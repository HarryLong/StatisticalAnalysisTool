#include "distribution_factory.h"
#include "math.h"
#include <map>
#include "analysis_point.h"
#include "point_map.h"

DistributionFactory::DistributionFactory() : m_dice_roller(0, RAND_MAX)
{

}

std::vector<AnalysisPoint*> DistributionFactory::generateRandomDistribution(int category_id, int p_number_of_points, int width, int height, int min_radius, int max_radius)
{
    PointMap points_taken;
    std::vector<AnalysisPoint*> ret;
    for(int i = 0; i < p_number_of_points; i++)
    {
        QPoint p(get_random_position(width, height));
        if(!points_taken.containsPoint(p))
        {
            points_taken.insertPoint(p);
            ret.push_back(new AnalysisPoint(category_id, p, get_random_radius(min_radius, max_radius)));
        }
    }
    return ret;
}

std::vector<AnalysisPoint*> DistributionFactory::generateGriddedDistribution(int category_id, int p_grid_seperation, int width, int height, int min_radius, int max_radius)
{
    std::vector<AnalysisPoint*> ret;
    for(int x(0); x < width; x+= p_grid_seperation)
        for(int y(0); y < height; y+= p_grid_seperation)
            ret.push_back(new AnalysisPoint(category_id, QPoint(x, y), get_random_radius(min_radius, max_radius)));

    return ret;
}

#define DEGREES_TO_RADIANS_MULTIPLIER (M_PI/180)
std::vector<AnalysisPoint*> DistributionFactory::generateSeededDistribution(int category_id, int n_seeds, int n_seeding_iterations, int max_seeding_distance,
                                                                    int width, int height, int min_radius, int max_radius, bool equidistant)
{
    std::vector<AnalysisPoint*> seeds;

    if(equidistant)
    {
        int seperation (((float)width) / n_seeds);

        for(int i(0); i <= n_seeds; i++)
        {
            seeds.push_back(new AnalysisPoint(category_id, QPoint(i*seperation, i*seperation),1));
        }
    }
    else
    {
        seeds = generateRandomDistribution(category_id, n_seeds, width, height);
    }

    std::vector<AnalysisPoint*> all;

    PointMap points_taken;

    for(AnalysisPoint* seed : seeds)
        points_taken.insertPoint(seed->getCenter());

    DiceRoller m_angle_dice_roller(0, 360);
    for( int i(0); i < n_seeding_iterations; i++ )
    {
        for( AnalysisPoint * seed : seeds )
        {
            int distance(m_dice_roller.generate()% max_seeding_distance);

            if(distance > 0)
            {
                float angle_in_radians(m_angle_dice_roller.generate() * DEGREES_TO_RADIANS_MULTIPLIER);

                QPoint diff(cos(angle_in_radians) * distance, sin(angle_in_radians) * distance);
                QPoint position(QPoint(seed->getCenter() + diff));
                if(position.x() > 0 && position.y() > 0 && position.x() < width && position.y() < height &&
                        !points_taken.containsPoint(position))
                {
                    points_taken.insertPoint(position);
                    all.push_back(new AnalysisPoint(category_id, position,get_random_radius(min_radius, max_radius)));
                }
            }
        }
    }

    // Add the seeds
    all.insert(all.begin()+all.size(), seeds.begin(), seeds.end());

    return all;
}

QPoint DistributionFactory::get_random_position(int max_width, int max_height)
{
    return QPoint(m_dice_roller.generate()%max_width, m_dice_roller.generate()%max_height);
}

int DistributionFactory::get_random_radius(int min, int max)
{
    int diff(max - min);
    if(diff > 0)
        return (min + (m_dice_roller.generate()%(diff+1)));

    return min;
}
