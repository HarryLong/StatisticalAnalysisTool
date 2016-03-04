#include "test_data_factory.h"

#include "../reproducer/distribution_factory.h"
#include "../analyser/analyzer.h"
#include "../analyser/radial_distribution_analyzer.h"

RadialDistribution * TestDataFactory::generateSingleCategoryRadialDistribution(AnalysisConfiguration & analysis_config,
                                                                               int n_points,
                                                                               int category_id,
                                                                               std::pair<int,int> size_range)
{
    // Generate analysis configuration
    std::vector<int> category_ids;
    category_ids.push_back(category_id);
    analysis_config.setPrioritySortedCategoryIds(category_ids);

    // Create points
    DistributionFactory distribution_factory;
    std::vector<AnalysisPoint> points (distribution_factory.generateRandomDistribution(category_id, n_points, analysis_config.analysis_window_width,
                                                                                       analysis_config.analysis_window_height,
                                                                                       size_range.first, size_range.second, 1.0f, 1.0f));

    // Create analyser
    RadialDistributionAnalyzer analyser (  analysis_config, points, points, category_id, category_id );
    analyser.calculateRadialDistribution(false);
    RadialDistribution * radial_distribution ( new RadialDistribution(analyser.getRadialDistribution()) );

//    for(AnalysisPoint * p : points)
//        delete p;

    return radial_distribution;
}
