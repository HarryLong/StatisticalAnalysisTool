#ifndef CATEGORIZED_DISTRIBUTION_ANALYZER_H
#define CATEGORIZED_DISTRIBUTION_ANALYZER_H

#include <map>
#include "radial_distribution.h"
#include <vector>

typedef std::map<std::pair<int, int>, RadialDistribution > PairCoorelations;
struct CategorizedRadialDistributionData{
    PairCoorelations pair_correlations;
    std::map<int,int> id_priorities;
};

class CategorizedDistributionAnalyzer{
    static generateCategorizedRadialDistribution()
};

#endif // CATEGORIZED_DISTRIBUTION_ANALYZER_H
