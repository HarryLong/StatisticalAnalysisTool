#include "analyzer.h"

#include "../analysis_point.h"
#include "dependency_analyzer.h"

#include "../utils/file_utils.h"
#include "../utils/utils.h"

#include <iostream>
#include <thread>

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
void Analyzer::analyze(QString base_directory, const std::map<int, std::vector<AnalysisPoint> > & points, AnalysisConfiguration configuration)
{
    Analyzer(base_directory, points, configuration).analyze();
}

Analyzer::Analyzer(QString base_directory, const std::map<int, std::vector<AnalysisPoint> > & points, AnalysisConfiguration analysis_configuration) :
    m_analysis_conf(analysis_configuration), m_points(points)
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
        input_file.append("input.png");
        ImageUtils::printPointsToImg(input_file.toStdString(), m_points, m_analysis_conf.analysis_window_width, m_analysis_conf.analysis_window_height);
        std::cout << "Input image written to: " << input_file.toStdString() << std::endl;
    }
}

Analyzer::~Analyzer()
{
}

void Analyzer::analyze()
{
    // Generate the configuration file
    generate_configuration();

    // Generate the pair correlations
    generate_pair_correlations();

    // Generate the category properties
    generate_category_properties();
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

void Analyzer::generate_pair_correlations()
{
    std::vector<RadialDistributionAnalyzer*> radial_distribution_analyzers;
    std::vector<RadialDistributionTracker*> radial_distribution_trackers;

    for(auto reference_category(m_analysis_conf.priority_sorted_category_ids.begin());
        reference_category != m_analysis_conf.priority_sorted_category_ids.end(); reference_category++)
    {
        int reference_category_id(*reference_category);
        std::vector<AnalysisPoint> reference_category_points(m_points.find(reference_category_id)->second);

        for(auto target_category(std::find(m_analysis_conf.priority_sorted_category_ids.begin(), m_analysis_conf.priority_sorted_category_ids.end(), reference_category_id));
                            target_category != m_analysis_conf.priority_sorted_category_ids.end(); target_category++)
        {
            int target_category_id(*target_category);
            std::vector<AnalysisPoint> target_category_points(m_points.find(target_category_id)->second);

            RadialDistributionTracker * tracker = new RadialDistributionTracker(m_radial_distribution_dir,
                                                                                 m_csv_dir);
            RadialDistributionAnalyzer * analyzer = new RadialDistributionAnalyzer(m_analysis_conf, reference_category_points, target_category_points,
                                              reference_category_id, target_category_id, tracker);

            radial_distribution_analyzers.push_back(analyzer);
            radial_distribution_trackers.push_back(tracker);

            analyzer->calculateRadialDistribution();
        }
    }

    // Wait for all the radial distribution analysis workers to be done before returning
    {
        bool workers_complete(false);
        while (!workers_complete)
        {
            auto it(radial_distribution_trackers.begin());
            workers_complete = (it == radial_distribution_trackers.end() || (*it)->m_complete.load());
            while(workers_complete && (++it != radial_distribution_trackers.end()))
                 workers_complete = workers_complete && (*it)->m_complete.load();

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
