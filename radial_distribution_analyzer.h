#ifndef RADIAL_DISTRIBUTION_ANALYZER_H
#define RADIAL_DISTRIBUTION_ANALYZER_H

#include <map>
#include <QPoint>
#include <vector>
#include "radial_distribution.h"
#include "analysis_point.h"

class TestClass;

class RadialDistributionAnalyzer{
public:
    RadialDistributionAnalyzer(int r_min, int r_max, int r_diff);
    ~RadialDistributionAnalyzer();

    RadialDistribution getRadialDistribution(std::vector<AnalysisPoint*> reference_points, std::vector<AnalysisPoint*> target_points, int width, int height,
                                             int reference_points_id = 0, int destination_points_id = 0);

    static bool overflows_border(QPoint center, int r_bracket, int r_diff, int width, int height);
    static double calculate_bordered_circle_area(QPoint center, int radius, int width, int height);

private:
    int r_diff;
    int r_min, r_max;
    std::map<int,double> m_annular_shell_areas;
    friend class TestClass;
};

#endif // RADIAL_DISTRIBUTION_ANALYZER_H
