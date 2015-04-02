#ifndef RADIAL_DISTRIBUTION_PRODUCER_H
#define RADIAL_DISTRIBUTION_PRODUCER_H

#include "radial_distribution.h"
#include <string>
#include <QPoint>

class PointMap : protected std::map<std::pair<int,int>,int>
{
public:
    PointMap();
    void insertPoint(QPoint p);
    void removePoint(QPoint p);
    bool containsPoint(QPoint p);
    std::map<std::pair<int,int>,int>::iterator begin();
    std::map<std::pair<int,int>,int>::iterator end();
};

class SpatialHashMapCell {
public:
    SpatialHashMapCell() : points() { }
//    SpatialHashMapCell(QPoint p) : points() { points.push_back(p); }
    std::vector<QPoint> points;
};

class RadialDistributionProducer
{
public:
    RadialDistributionProducer(std::string radial_distribution_file);
    RadialDistributionProducer(RadialDistribution distribution);

    std::vector<QPoint> generatePoints(int width, int height, int iterations, bool verbose = false);

    ~RadialDistributionProducer();

private:
    std::vector<QPoint> initialize(int width, int height);
    QPoint get_random_point(int max_width, int max_height);
    float calculateStrength(const QPoint & reference_point, const std::vector<QPoint> & destination_points);
    float calculateStrength(const QPoint & reference_point, const std::vector<SpatialHashMapCell*> & destination_points);

    RadialDistribution m_radial_distribution;
};

#endif
