#include "distribution_factory.h"
#include "math.h"
#include <map>

DistributionFactory::DistributionFactory() : m_dice_roller(0, RAND_MAX)
{

}

std::vector<QPoint> DistributionFactory::generateRandomDistribution(int p_number_of_points, int width, int height)
{
    std::map<std::pair<int,int>, int> points_taken;
    std::vector<QPoint> ret;
    for(int i = 0; i < p_number_of_points; i++)
    {
        QPoint p(m_dice_roller.generate()%width, m_dice_roller.generate()%height);
        if(points_taken.find(std::pair<int,int>(p.x(),p.y())) == points_taken.end())
        {
            points_taken.insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(p.x(),p.y()),1));
            ret.push_back(QPoint(m_dice_roller.generate()%width, m_dice_roller.generate()%height));
        }
    }
    return ret;
}

std::vector<QPoint> DistributionFactory::generateGriddedDistribution(int p_grid_seperation, int width, int height)
{
    std::vector<QPoint> ret;
    for(int x(0); x < width; x+= p_grid_seperation)
        for(int y(0); y < height; y+= p_grid_seperation)
            ret.push_back(QPoint(x, y));

    return ret;
}

#define DEGREES_TO_RADIANS_MULTIPLIER (M_PI/180)
std::vector<QPoint> DistributionFactory::generateSeededDistribution(int n_seeds, int n_seeding_iterations, int max_seeding_distance, int width, int height)
{
    std::vector<QPoint> seeds(generateRandomDistribution(n_seeds, width, height));
    std::vector<QPoint> all;

    std::map<std::pair<int,int>, int> points_taken;
    for(QPoint seed : seeds)
        points_taken.insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(seed.x(),seed.y()),1));

    DiceRoller m_angle_dice_roller(0, 360);
    for( int i(0); i < n_seeding_iterations; i++ )
    {
        for( QPoint seed : seeds )
        {
            int distance(m_dice_roller.generate()% max_seeding_distance);

            if(distance > 0)
            {
                float angle_in_radians(m_angle_dice_roller.generate() * DEGREES_TO_RADIANS_MULTIPLIER);

                QPoint diff(cos(angle_in_radians) * distance, sin(angle_in_radians) * distance);
                QPoint position(QPoint(seed + diff));
                if(position.x() > 0 && position.y() > 0 && position.x() < width && position.y() < height &&
                        (points_taken.find(std::pair<int,int>(position.x(),position.y())) == points_taken.end()))
                {
                    points_taken.insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(position.x(),position.y()),1));
                    all.push_back(position);
                }
            }
        }
    }

    // Add the seeds
    all.insert(all.begin()+all.size(), seeds.begin(), seeds.end());

    return all;
}
