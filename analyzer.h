#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include <map>
#include <QString>
#include "analysis_configuration.h"

class AnalysisPoint;
namespace Analyzer{
    void generate_statistical_data(QString directory, std::map<int, std::vector<AnalysisPoint*> > & points, AnalysisConfiguration configuration,
                                   bool write_human_readable_files = false);
}

#endif // ANALYZER_H
