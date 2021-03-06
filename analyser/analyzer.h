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

class QProgressBar;
class AnalysisPoint;
class RadialDistributionTracker;
class Analyzer{
public:
     static unsigned long analyze(QString base_directory, const std::map<int, std::vector<AnalysisPoint> > & points, AnalysisConfiguration configuration, QProgressBar * progress_bar = nullptr);

private:
    Analyzer(QString base_directory, const std::map<int, std::vector<AnalysisPoint> > & points, AnalysisConfiguration configuration,
             unsigned long timestamp, QProgressBar * progress_bar = nullptr);
    ~Analyzer();

    void analyze();

    void generate_configuration();
    void generate_pair_correlations();
    void generate_category_properties();
    void generate_timestamp();

    QString m_base_dir;
    QString m_radial_distribution_dir;
    QString m_cateory_properties_dir;
    QString m_csv_dir;
    QProgressBar * m_progress_bar;
    const AnalysisConfiguration m_analysis_conf;
    const std::map<int, std::vector<AnalysisPoint> > m_points;
    unsigned int m_timestamp;
};

/*******************************
 * RADIAL DISTRIBUTION TRACKER *
 *******************************/
class RadialDistributionTracker : public RadialDistributionAnalyzer::CompletionListener{
public:
    RadialDistributionTracker(QString m_output_bin_dir, QString m_output_human_readable_file);
    virtual ~RadialDistributionTracker();
    virtual void complete(const RadialDistribution & radial_distribution);

    const QString m_output_bin_dir;
    const QString m_output_hr_dir;
    std::atomic<bool> m_complete;
};

/*******************************
 * CATEGORY PROPERTIES TRACKER *
 *******************************/
class CategoryPropertiesTracker: public CategoryAnalyzer::CompletionListener{
public:
    CategoryPropertiesTracker(QString m_output_bin_dir, QString m_output_human_readable_file, std::set<int> category_dependencies);
    virtual~CategoryPropertiesTracker();
    virtual void complete(CategoryProperties & category_properties);

    const QString m_output_bin_dir;
    const QString m_output_hr_dir;
    std::set<int> m_category_dependencies;
    std::atomic<bool> m_complete;
};

#endif // ANALYZER_H
