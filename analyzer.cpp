#include "analyzer.h"

#include "analysis_point.h"
#include "radial_distribution_analyzer.h"
#include "constants.h"
#include "category_analyzer.h"
#include "utils.h"
#include "analysis_point.h"

#include <iostream>

void Analyzer::generate_statistical_data(QString directory, std::map<int, std::vector<AnalysisPoint*> > points, AnalysisConfiguration analysis_configuration,
                                         bool write_human_readable_files)
{
    // Init the directory structure
    if(!directory.endsWith("/"))
        directory.append("/");

    QString radial_distributions_folder, category_properties_folder, csv_files_folder;

    if(!FileUtils::init_directory_structure(directory, radial_distributions_folder, category_properties_folder, csv_files_folder ))
    {
        std::cerr << "An error occured whilst creating the directory structure..." << std::endl;
        return;
    }

    // Write the configuration file
    {
        QString generic_filename(CONFIGURATION_FILE_NAME);

        // Write bin file
        {
            QString output_filename(directory);
            output_filename.append(generic_filename);
            analysis_configuration.write(output_filename.toStdString());
            std::cout << "Configuration file written to: " << output_filename.toStdString() << std::endl;
        }
        // Write CSV file
        if(write_human_readable_files)
        {
            QString output_filename(csv_files_folder);
            output_filename.append(generic_filename).append(".csv");
            analysis_configuration.writeToCSV(output_filename.toStdString());
            std::cout << "Configuration CSV file written to: " << output_filename.toStdString() << std::endl;
        }
    }

    // Generate the radial distribution files
    RadialDistributionAnalyzer radial_distribution_analyzer(analysis_configuration);
    std::vector<RadialDistribution> radial_distributions;
    for(auto category_1(analysis_configuration.priority_sorted_category_ids.begin());
        category_1 != analysis_configuration.priority_sorted_category_ids.end(); category_1++)
    {
        int category_1_id(*category_1);
        std::vector<AnalysisPoint*> category_1_points(points.find(category_1_id)->second);

        for(auto category_2(std::find(analysis_configuration.priority_sorted_category_ids.begin(), analysis_configuration.priority_sorted_category_ids.end(), category_1_id));
                            category_2 != analysis_configuration.priority_sorted_category_ids.end(); category_2++)
        {
            int category_2_id(*category_2);
            QString generic_filename("category_");
            generic_filename.append(QString::number(category_1_id)).append("_and_").append(QString::number(category_2_id));

            std::vector<AnalysisPoint*> category_2_points(points.find(category_2_id)->second);

            RadialDistribution radial_distribution(radial_distribution_analyzer.getRadialDistribution(category_1_points, category_2_points, category_1_id, category_2_id));
            radial_distributions.push_back(radial_distribution);

            // Write rad file
            {
                QString output_filename(radial_distributions_folder);
                output_filename.append(generic_filename).append(RADIAL_DISTRIBUTION_FILE_EXTENSION);
                radial_distribution.write(output_filename.toStdString());
                std::cout << "Radial distribution file for categories: " << category_1_id << " and " << category_2_id <<
                             " written to file: " << output_filename.toStdString() << std::endl;
            }
            // Write CSV file
            if(write_human_readable_files)
            {
                QString output_filename(csv_files_folder);
                output_filename.append(generic_filename).append("_pair_correlation").append(".csv");
                radial_distribution.writeToCSV(output_filename.toStdString());
                std::cout << "CSV distribution file for categories: " << category_1_id << " and " << category_2_id <<
                             " written to file: " << output_filename.toStdString() << std::endl;
            }
        }
    }
    // Generate the category properties
    CategoryAnalyzer category_analyzer;
    int priority(1);
    for(auto it(analysis_configuration.priority_sorted_category_ids.begin()); it != analysis_configuration.priority_sorted_category_ids.end(); it++)
    {
        int category_id(*it);

        QString generic_filename("category_");
        generic_filename.append(QString::number(category_id));

        std::vector<AnalysisPoint*> category_points(points.find(category_id)->second);

        CategoryProperties category_properties(category_analyzer.getCategoryProperties(category_points, category_id, priority++));

        // Write category properties file
        {
            QString output_filename(category_properties_folder);
            output_filename.append(generic_filename).append(CATEGORY_PROPERTIES_FILE_EXTENSION);
            category_properties.write(output_filename.toStdString());
            std::cout << "Category properties file for category " << category_id << " written to: " << output_filename.toStdString() << std::endl;
        }

        if(write_human_readable_files)
        {
            QString output_filename(csv_files_folder);
            output_filename.append(generic_filename).append("_size_properties").append(".csv");
            category_properties.writeToCSV(output_filename.toStdString());
            std::cout << "Category properties CSV file for category " << category_id << " written to: " << output_filename.toStdString() << std::endl;
        }
    }
}

