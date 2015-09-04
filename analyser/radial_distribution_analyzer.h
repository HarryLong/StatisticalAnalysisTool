#ifndef RADIAL_DISTRIBUTION_ANALYZER_H
#define RADIAL_DISTRIBUTION_ANALYZER_H

#include <map>
#include <QPoint>
#include <vector>
#include "../radial_distribution.h"
#include "../analysis_point.h"
#include "analysis_configuration.h"
#include <thread>

class RadialDistributionCompletionListener{
public:
    virtual void complete(const RadialDistribution & radial_distribution) = 0;
};

class RadialDistributionAnalyzer{
public:
    RadialDistributionAnalyzer(AnalysisConfiguration analysis_configuration, const std::vector<AnalysisPoint*> reference_points,
                               const std::vector<AnalysisPoint*> target_points, int reference_points_id, int destination_points_id,
                               RadialDistributionCompletionListener * m_completion_listener = NULL);
    ~RadialDistributionAnalyzer();

    void calculateRadialDistribution(bool asynchronous = true);

    RadialDistribution getRadialDistribution();

    static bool overflows_border(QPoint center, int radius, int window_width, int window_height);
    static double calculate_bordered_circle_area(QPoint center, int radius, int width, int height);

    const AnalysisConfiguration m_analysis_configuration;
    const std::vector<AnalysisPoint*> m_reference_points;
    const std::vector<AnalysisPoint*> m_target_points;
    const int m_reference_points_id;
    const int m_target_points_id;
private:
    void calculate_radial_distribution();
    RadialDistribution m_radial_distribution;
    RadialDistributionCompletionListener * m_completion_listener;
    std::thread * m_worker;
};

#endif // RADIAL_DISTRIBUTION_ANALYZER_H
