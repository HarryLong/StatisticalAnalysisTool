#ifndef DISTRIBUTION_FACTORY_H
#define DISTRIBUTION_FACTORY_H

#include "dice_roller.h"

#include <QPoint>
class DistributionFactory
{
public:
    DistributionFactory();
    std::vector<QPoint> generateRandomDistribution(int p_number_of_points, int width, int height);
    std::vector<QPoint> generateGriddedDistribution(int p_grid_seperation, int width, int height);
    std::vector<QPoint> generateSeededDistribution(int n_seeds, int n_seeding_iterations, int max_seeding_distance, int width, int height);
private:
    DiceRoller m_dice_roller;
};


#endif // DISTRIBUTION_FACTORY_H
