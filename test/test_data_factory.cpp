#include "test_data_factory.h"

#include "../distribution_factory.h"
#include "../analyzer.h"
#include "../radial_distribution_analyzer.h"

RadialDistribution * TestDataFactory::generateSingleCategoryRadialDistribution(AnalysisConfiguration * analysis_config,
                                                                               int n_points,
                                                                               int category_id,
                                                                               std::pair<int,int> size_range)
{
    // Generate analysis configuration
    std::vector<int> category_ids;
    category_ids.push_back(category_id);
    analysis_config->setPrioritySortedCategoryIds(category_ids);

    // Create analyser
    RadialDistributionAnalyzer analyser (*analysis_config);

    // Create points
    DistributionFactory distribution_factory;
    std::vector<AnalysisPoint*> points (distribution_factory.generateRandomDistribution(category_id, n_points, analysis_config->analysis_window_width, analysis_config->analysis_window_height,
                                                                                        size_range.first, size_range.second));

    bool dependent;
    return new RadialDistribution(analyser.getRadialDistribution(points, points, category_id, category_id, dependent));
}

AnalysisConfiguration* TestDataFactory::generateAnalysisConfiguration(int r_min, int r_max, int r_diff, int analysis_window_width, int analysis_window_height)
{
    return new AnalysisConfiguration(
                r_min,
                r_max,
                r_diff,
                analysis_window_width,
                analysis_window_height);
}
