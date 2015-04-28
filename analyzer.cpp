#include "analyzer.h"

#include "analysis_point.h"
#include "radial_distribution_analyzer.h"
#include "constants.h"
#include "category_analyzer.h"
#include "utils.h"
#include "analysis_point.h"

#include <iostream>

void Analyzer::generate_statistical_data(QString directory, std::map<int, std::vector<AnalysisPoint*> > & points, AnalysisConfiguration analysis_configuration,
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

    // Input
    {
        QString input_file(directory);
        input_file.append("input.png");
        ImageUtils::printPointsToImg(input_file.toStdString(), points, analysis_configuration.analysis_window_width, analysis_configuration.analysis_window_height);
        std::cout << "Input image written to: " << input_file.toStdString() << std::endl;
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
            std::cout << __LINE__ << std::endl;
            std::cout << "Configuration CSV file written to: " << output_filename.toStdString() << std::endl;
            std::cout << __LINE__ << std::endl;
        }
        std::cout << __LINE__ << std::endl;
    }

    // Generate the radial distribution files
    std::cout << __LINE__ << std::endl;
    std::map<int,std::vector<int>> dependent_categories;
    std::cout << __LINE__ << std::endl;

    RadialDistributionAnalyzer radial_distribution_analyzer(analysis_configuration);
    std::vector<RadialDistribution> radial_distributions;
    for(auto target_category(analysis_configuration.priority_sorted_category_ids.begin());
        target_category != analysis_configuration.priority_sorted_category_ids.end(); target_category++)
    {
        std::cout << __LINE__ << std::endl;
        int target_category_id(*target_category);
        std::vector<AnalysisPoint*> target_category_points(points.find(target_category_id)->second);
        std::cout << __LINE__ << std::endl;

        for(auto reference_category(std::find(analysis_configuration.priority_sorted_category_ids.begin(), analysis_configuration.priority_sorted_category_ids.end(), target_category_id));
                            reference_category != analysis_configuration.priority_sorted_category_ids.end(); reference_category++)
        {
            int reference_category_id(*reference_category);
            QString generic_filename("category_");
            generic_filename.append(QString::number(reference_category_id)).append("_and_").append(QString::number(target_category_id));

            std::vector<AnalysisPoint*> reference_category_points(points.find(reference_category_id)->second);

            bool dependent;
            RadialDistribution radial_distribution(radial_distribution_analyzer.getRadialDistribution(reference_category_points, target_category_points,
                                                                                                      reference_category_id, target_category_id, dependent));

            if(dependent)
            {
                if(dependent_categories.find(reference_category_id) == dependent_categories.end())
                    dependent_categories.insert(std::pair<int,std::vector<int> >(reference_category_id, std::vector<int>()));

                dependent_categories[reference_category_id].push_back(target_category_id);
            }

            radial_distributions.push_back(radial_distribution);

            // Write rad file
            {
                QString output_filename(radial_distributions_folder);
                output_filename.append(generic_filename).append(RADIAL_DISTRIBUTION_FILE_EXTENSION);
                radial_distribution.write(output_filename.toStdString());
                std::cout << "Radial distribution file for categories: " << reference_category_id << " and " << target_category_id <<
                             " written to file: " << output_filename.toStdString() << std::endl;
            }
            // Write CSV file
            if(write_human_readable_files)
            {
                QString output_filename(csv_files_folder);
                output_filename.append(generic_filename).append("_pair_correlation").append(".csv");
                radial_distribution.writeToCSV(output_filename.toStdString());
                std::cout << "CSV distribution file for categories: " << reference_category_id << " and " << target_category_id <<
                             " written to file: " << output_filename.toStdString() << std::endl;
            }
        }
    }
    std::cout << __LINE__ << std::endl;

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

        // Dependent category ids
        auto dependent_categories_it(dependent_categories.find(category_id));
        if(dependent_categories_it != dependent_categories.end())
        {
            category_properties.m_header.category_dependent_ids = dependent_categories_it->second;
        }

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
            output_filename.append(generic_filename).append("_properties").append(".csv");
            category_properties.writeToCSV(output_filename.toStdString());
            std::cout << "Category properties CSV file for category " << category_id << " written to: " << output_filename.toStdString() << std::endl;
        }
    }
    std::cout << __LINE__ << std::endl;
}

