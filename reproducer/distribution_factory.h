#ifndef DISTRIBUTION_FACTORY_H
#define DISTRIBUTION_FACTORY_H

#include "../utils/dice_roller.h"

#include <QPoint>

class AnalysisPoint;
class DistributionFactory
{
public:
    DistributionFactory();
    std::vector<AnalysisPoint> generateRandomDistribution(int category_id, int p_number_of_points, int width, int height,
                                                          int min_height, int max_height, float canopy_r_multiplier, float root_size_multiplier);
    std::vector<AnalysisPoint> generateGriddedDistribution(int category_id, int p_grid_seperation, int width, int height,
                                                           int min_height, int max_height, float canopy_r_multiplier, float root_size_multiplier);
    std::vector<AnalysisPoint> generateSeededDistribution(int category_id, int n_seeds, int n_seeding_iterations, int max_seeding_distance,
                                                            int width, int height, int min_height, int max_height, float canopy_r_multiplier, float root_size_multiplier,
                                                            bool equidistant = false);
private:
    QPoint get_random_position(int max_width, int max_height);
    AnalysisPoint generate_analysis_point(int category_id, QPoint position, int min_height, int max_height, float canopy_radius_multiplier, float root_size_multiplier);
    int get_random_value(int min, int max);

    DiceRoller m_dice_roller;
};


#endif // DISTRIBUTION_FACTORY_H
