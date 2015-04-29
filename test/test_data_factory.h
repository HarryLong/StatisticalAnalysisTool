#include "../radial_distribution.h"
#include "../analysis_configuration.h"

class TestDataFactory{
public:
    static RadialDistribution * generateSingleCategoryRadialDistribution(AnalysisConfiguration & analysis_config,
                                                                         int n_points,
                                                                         int category_id,
                                                                         std::pair<int,int> size_range = std::pair<int,int>(1,1));
};
