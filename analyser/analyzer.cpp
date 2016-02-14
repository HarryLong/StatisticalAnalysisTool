#include "analyzer.h"

#include "../analysis_point.h"
#include "dependency_analyzer.h"

#include "../utils/file_utils.h"
#include "../utils/utils.h"

#include <iostream>
#include <thread>
#include <QProgressBar>
#include <fstream>

#include <chrono>

/*******************************
 * RADIAL DISTRIBUTION TRACKER *
 *******************************/
RadialDistributionTracker::RadialDistributionTracker(QString output_bin_dir, QString output_hr_dir) :
    m_output_bin_dir(output_bin_dir), m_output_hr_dir(output_hr_dir)
{
    m_complete.store(false);
}

RadialDistributionTracker::~RadialDistributionTracker()
{

}

void RadialDistributionTracker::complete(const RadialDistribution & radial_distribution)
{
    int reference_category_id(radial_distribution.m_header.reference_id);
    int target_category_id(radial_distribution.m_header.destination_id);

    QString filename("category_");
    filename.append(QString::number(reference_category_id)).append("_and_").append(QString::number(target_category_id));

    // Write rad file
    {
        QString output_filename(m_output_bin_dir);
        output_filename.append(filename).append(FileUtils::_RADIAL_DISTRIBUTION_EXT);
        radial_distribution.write(output_filename.toStdString());
        std::cout << "Radial distribution file for categories: " << target_category_id << " and " << reference_category_id <<
                     " written to file: " << output_filename.toStdString() << std::endl;
    }
    // Write CSV file
    {
        QString output_filename(m_output_hr_dir);
        output_filename.append(filename).append("_pair_correlation").append(".csv");
        radial_distribution.writeToCSV(output_filename.toStdString());
        std::cout << "CSV distribution file for categories: " << target_category_id << " and " << reference_category_id <<
                     " written to file: " << output_filename.toStdString() << std::endl;
    }
    m_complete.store(true);
}

/*******************************
 * CATEGORY PROPERTIES TRACKER *
 *******************************/
CategoryPropertiesTracker::CategoryPropertiesTracker(QString output_bin_dir, QString output_hr_dir, std::set<int> category_dependencies) :
    m_output_bin_dir(output_bin_dir), m_output_hr_dir(output_hr_dir), m_category_dependencies(category_dependencies)
{
    m_complete.store(false);
}

CategoryPropertiesTracker::~CategoryPropertiesTracker()
{

}

void CategoryPropertiesTracker::complete(CategoryProperties & category_properties)
{
    int category_id(category_properties.m_header.category_id);

    QString filename("category_");
    filename.append(QString::number(category_id));

    category_properties.m_header.category_dependent_ids = m_category_dependencies;

    // Write category properties file
    {
        QString output_filename(m_output_bin_dir);
        output_filename.append(filename).append(FileUtils::_CATEGORY_PROPERTIES_EXT);
        category_properties.write(output_filename.toStdString());
        std::cout << "Category properties file for category " << category_id << " written to: " << output_filename.toStdString() << std::endl;
    }
    // human readable
    {
        QString output_filename(m_output_hr_dir);
        output_filename.append(filename).append("_properties").append(".csv");
        category_properties.writeToCSV(output_filename.toStdString());
        std::cout << "Category properties CSV file for category " << category_id << " written to: " << output_filename.toStdString() << std::endl;
    }
    m_complete.store(true);
}

/************
 * ANALYZER *
 ************/
unsigned long Analyzer::analyze(QString base_directory, const std::map<int, std::vector<AnalysisPoint> > & points, AnalysisConfiguration configuration,
                                QProgressBar * progress_bar)
{
    unsigned long timestamp( std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    Analyzer(base_directory, points, configuration, timestamp, progress_bar).analyze();

    return timestamp;
}

Analyzer::Analyzer(QString base_directory, const std::map<int, std::vector<AnalysisPoint> > & points, AnalysisConfiguration analysis_configuration,
                    unsigned long timestamp, QProgressBar * progress_bar) :
    m_analysis_conf(analysis_configuration), m_points(points), m_progress_bar(progress_bar), m_timestamp(timestamp)
{
    // Init the base directory
    if(!base_directory.endsWith("/"))
        base_directory.append("/");
    m_base_dir = base_directory;

    if(!FileUtils::init_directory_structure(m_base_dir, m_radial_distribution_dir, m_cateory_properties_dir, m_csv_dir))
    {
        std::cerr << "An error occured whilst creating the directory structure..." << std::endl;
        return;
    }

    // Write the input image
    {
        QString input_file(m_base_dir);
        input_file.append("input");
        ImageUtils::printPointsToImg(input_file.toStdString(), m_points, m_analysis_conf.analysis_window_width, m_analysis_conf.analysis_window_height);
        std::cout << "Input image written to: " << input_file.toStdString() << std::endl;
    }
}

Analyzer::~Analyzer()
{
}

void Analyzer::analyze()
{
    std::cout << "Starting statistican analysis" << std::endl;

    std::chrono::high_resolution_clock::time_point start_time( std::chrono::high_resolution_clock::now() );

    // Generate the configuration file
    generate_configuration();

    // Generate the pair correlations
    generate_pair_correlations();

    // Generate the category properties
    generate_category_properties();

    generate_timestamp();

    auto duration (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count());

    std::cout << "Duration: " << duration << " milliseconds." << std::endl;
}

void Analyzer::generate_timestamp()
{
    std::string filename(m_base_dir.append(FileUtils::_TIMESTAMP_FILENAME).toStdString());

    // Write timestamp file
    std::ofstream file;
    file.open(filename);
    if(file.is_open())
        file << m_timestamp << "\n";
    file.close();
}

void Analyzer::generate_configuration()
{
    QString generic_filename(FileUtils::_CONFIGURATION_FILENAME);

    // Write bin file
    {
        QString output_filename(m_base_dir);
        output_filename.append(generic_filename);
        m_analysis_conf.write(output_filename.toStdString());
        std::cout << "Configuration file written to: " << output_filename.toStdString() << std::endl;
    }
    // Write CSV file
    {
        QString output_filename(m_csv_dir);
        output_filename.append(generic_filename).append(".csv");
        m_analysis_conf.writeToCSV(output_filename.toStdString());
        std::cout << "Configuration CSV file written to: " << output_filename.toStdString() << std::endl;
    }
}

#include <QDebug>
void Analyzer::generate_pair_correlations()
{
    std::vector<RadialDistributionAnalyzer*> radial_distribution_analyzers;
    std::vector<RadialDistributionTracker*> radial_distribution_trackers;

    int n_radial_distributions(0);

    for(auto reference_category(m_analysis_conf.priority_sorted_category_ids.begin());
        reference_category != m_analysis_conf.priority_sorted_category_ids.end(); reference_category++)
    {
        int reference_category_id(*reference_category);
        std::vector<AnalysisPoint> reference_category_points(m_points.find(reference_category_id)->second);

//        std::cout << "Reference category: " << reference_category_id << std::endl;

        for(auto target_category(std::find(m_analysis_conf.priority_sorted_category_ids.begin(), m_analysis_conf.priority_sorted_category_ids.end(), reference_category_id));
                            target_category != m_analysis_conf.priority_sorted_category_ids.end(); target_category++)
        {
            int target_category_id(*target_category);
//            std::cout << "Target category: " << target_category_id << std::endl;
            std::vector<AnalysisPoint> target_category_points(m_points.find(target_category_id)->second);

            RadialDistributionTracker * tracker = new RadialDistributionTracker(m_radial_distribution_dir,
                                                                                 m_csv_dir);
            RadialDistributionAnalyzer * analyzer = new RadialDistributionAnalyzer(m_analysis_conf, reference_category_points, target_category_points,
                                              reference_category_id, target_category_id, tracker);

            radial_distribution_analyzers.push_back(analyzer);
            radial_distribution_trackers.push_back(tracker);

            analyzer->calculateRadialDistribution();

            n_radial_distributions++;
        }
    }

    if(m_progress_bar)
        m_progress_bar->setRange(0,n_radial_distributions);

    // Wait for all the radial distribution analysis workers to be done before returning
    {
        bool workers_complete(false);
        while (!workers_complete)
        {
            int complete_count(0);
            for(auto it(radial_distribution_trackers.begin()); it != radial_distribution_trackers.end(); it++)
            {
                if((*it)->m_complete.load())
                    complete_count++;
            }
            workers_complete = (complete_count == n_radial_distributions);

            if(m_progress_bar)
                m_progress_bar->setValue(workers_complete);

            if(!workers_complete)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    for(RadialDistributionTracker * tracker : radial_distribution_trackers)
        delete tracker;
    for(RadialDistributionAnalyzer * analyzer : radial_distribution_analyzers)
        delete analyzer;
}

void Analyzer::generate_category_properties()
{
    DependencyAnalyzer::Dependencies dependencies(DependencyAnalyzer::getDependencies(m_points, m_analysis_conf.analysis_window_width,
                                                                                      m_analysis_conf.analysis_window_height));
    std::vector<CategoryAnalyzer*> category_analyzers;
    std::vector<CategoryPropertiesTracker*> category_properties_trackers;

    int priority(1);
    for(auto it(m_analysis_conf.priority_sorted_category_ids.begin()); it != m_analysis_conf.priority_sorted_category_ids.end(); it++)
    {
        int category_id(*it);
        std::vector<AnalysisPoint> category_points(m_points.find(category_id)->second);

        // Tracker
        std::set<int> dependent_categories;
        auto dependent_categories_it(dependencies.find(category_id));
        if(dependent_categories_it != dependencies.end())
            dependent_categories = dependent_categories_it->second;
        CategoryPropertiesTracker * tracker ( new CategoryPropertiesTracker(m_cateory_properties_dir, m_csv_dir, dependent_categories) );

        // Analyzer
        CategoryAnalyzer * analyzer (new CategoryAnalyzer(category_points, category_id, priority++, tracker));

        category_analyzers.push_back(analyzer);
        category_properties_trackers.push_back(tracker);

        analyzer->calculateCategoryProperties();
    }

    // Wait for all the category analysis workers to be done before returning
    {
        bool workers_complete(false);
        while (!workers_complete)
        {
            auto it(category_properties_trackers.begin());
            workers_complete = (it == category_properties_trackers.end() || (*it)->m_complete.load());
            while(workers_complete && (++it != category_properties_trackers.end()))
                 workers_complete = workers_complete && (*it)->m_complete.load();

            if(!workers_complete)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    for(CategoryPropertiesTracker * tracker : category_properties_trackers)
        delete tracker;
    for(CategoryAnalyzer * analyzer : category_analyzers)
        delete analyzer;
}
