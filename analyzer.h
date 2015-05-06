#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include <map>
#include <QString>
#include "analysis_configuration.h"
#include "radial_distribution_analyzer.h"
#include "category_analyzer.h"

#include <atomic>

class AnalysisPoint;
class RadialDistributionTracker;
class Analyzer{
public:
     static void analyze(QString base_directory, std::map<int, std::vector<AnalysisPoint*> > & points, AnalysisConfiguration configuration);

private:
    Analyzer(QString base_directory, std::map<int, std::vector<AnalysisPoint*> > & points, AnalysisConfiguration configuration);
    ~Analyzer();

    void generate_configuration();
    void generate_pair_correlations();
    void generate_category_properties();

    QString m_base_dir;
    QString m_radial_distribution_dir;
    QString m_cateory_properties_dir;
    QString m_csv_dir;
    const AnalysisConfiguration m_analysis_conf;
    const std::map<int, std::vector<AnalysisPoint*> > & m_points;
};

/*******************************
 * RADIAL DISTRIBUTION TRACKER *
 *******************************/
class RadialDistributionTracker : public RadialDistributionCompletionListener{
public:
    RadialDistributionTracker(QString m_output_bin_dir, QString m_output_human_readable_file);
    virtual void complete(const RadialDistribution & radial_distribution);

    const QString m_output_bin_dir;
    const QString m_output_hr_dir;
    std::atomic<bool> m_complete;
};

/*******************************
 * CATEGORY PROPERTIES TRACKER *
 *******************************/
class CategoryPropertiesTracker: public CategoryAnalysisCompletionListener{
public:
    CategoryPropertiesTracker(QString m_output_bin_dir, QString m_output_human_readable_file, std::set<int> category_dependencies);
    virtual void complete(CategoryProperties & category_properties);

    const QString m_output_bin_dir;
    const QString m_output_hr_dir;
    std::set<int> m_category_dependencies;
    std::atomic<bool> m_complete;
};

#endif // ANALYZER_H
