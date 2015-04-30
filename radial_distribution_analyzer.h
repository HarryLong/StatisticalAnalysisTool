#ifndef RADIAL_DISTRIBUTION_ANALYZER_H
#define RADIAL_DISTRIBUTION_ANALYZER_H

#include <map>
#include <QPoint>
#include <vector>
#include "radial_distribution.h"
#include "analysis_point.h"
#include "analysis_configuration.h"

class TestClass;

class RadialDistributionAnalyzer{
public:
    RadialDistributionAnalyzer(AnalysisConfiguration analysis_configuration);
    ~RadialDistributionAnalyzer();

    RadialDistribution getRadialDistribution(std::vector<AnalysisPoint*> & reference_points, std::vector<AnalysisPoint*> & target_points,
                                             int reference_points_id, int destination_points_id, bool & dependent);

    static bool overflows_border(QPoint center, int radius, int window_width, int window_height);
    static double calculate_bordered_circle_area(QPoint center, int radius, int width, int height);

private:
    AnalysisConfiguration m_analysis_configuration;
};

#endif // RADIAL_DISTRIBUTION_ANALYZER_H
