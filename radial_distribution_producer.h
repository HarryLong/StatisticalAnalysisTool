#ifndef RADIAL_DISTRIBUTION_PRODUCER_H
#define RADIAL_DISTRIBUTION_PRODUCER_H

#include "radial_distribution.h"
#include <string>
#include <QPoint>

class RadialDistributionProducer
{
public:
    RadialDistributionProducer(std::string radial_distribution_file);
    RadialDistributionProducer(RadialDistribution distribution);

    std::vector<QPoint> generatePoints(int width, int height, int iterations, bool verbose = false);

    ~RadialDistributionProducer();

private:
    std::vector<QPoint> initialize(int width, int height, float & strength);
    QPoint get_random_point(int max_width, int max_height);
    float calculateStrength(QPoint reference_point, std::vector<QPoint> destination_points, int & points_within_r_max);

    RadialDistribution m_radial_distribution;
};

#endif
