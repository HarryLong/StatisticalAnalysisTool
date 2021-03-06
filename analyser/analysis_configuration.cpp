#include "analysis_configuration.h"
#include "../utils/file_utils.h"

#include <iostream>
#include <fstream>

AnalysisConfiguration::AnalysisConfiguration(int r_min, int r_max, int r_diff, int analysis_window_width,
                                             int analysis_window_height, std::vector<int> priority_sorted_category_ids) :
    r_min(r_min),
    r_max(r_max),
    r_diff(r_diff),
    analysis_window_width(analysis_window_width),
    analysis_window_height(analysis_window_height),
    priority_sorted_category_ids(priority_sorted_category_ids)
{

}

AnalysisConfiguration::AnalysisConfiguration(std::string filename)
{
    load_data(filename);
}

bool AnalysisConfiguration::operator ==(const AnalysisConfiguration &other) const
{
    return  (r_min == other.r_min &&
             r_max == other.r_max &&
             r_diff == other.r_diff &&
             analysis_window_width == other.analysis_window_width &&
             analysis_window_height == other.analysis_window_height &&
             priority_sorted_category_ids == other.priority_sorted_category_ids);
}

bool AnalysisConfiguration::load_data(std::string filename)
{
    std::ifstream file;
    file.open(filename, std::ios_base::binary | std::ios_base::ate);

    if(file.is_open())
    {
        std::streampos size;
        size = file.tellg();

        file.seekg(0, std::ios_base::beg);

        // Check the signature
        {
            char * memblock = new char[CONFIGURATION_SIGNATURE_LENGTH];
            file.read(memblock, CONFIGURATION_SIGNATURE_LENGTH);
            memblock[CONFIGURATION_SIGNATURE_LENGTH] = '\0'; // Null-terminate

            std::string expected_signature(CONFIGURATION_SIGNATURE);
            if(strcmp(memblock, expected_signature.c_str()) != 0)
            {
                std::cerr << "File signature (" << memblock << ") is invalid!" << std::endl;
                delete [] memblock;
                return false;
            }

            delete [] memblock;
        }

        // Data
        {
            char * memblock = new char[4];

            // Points id
            file.read(memblock, 4);
            r_min = FileUtils::readInt32((unsigned char*) memblock);

            // Min
            file.read(memblock, 4);
            r_max = FileUtils::readInt32((unsigned char*) memblock);

            // Max
            file.read(memblock, 4);
            r_diff = FileUtils::readInt32((unsigned char*) memblock);

            // Diff
            file.read(memblock, 4);
            analysis_window_width = FileUtils::readInt32((unsigned char*) memblock);

            // N points
            file.read(memblock, 4);
            analysis_window_height = FileUtils::readInt32((unsigned char*) memblock);

            // N categories
            file.read(memblock, 4);
            int n_categories = FileUtils::readInt32((unsigned char*) memblock);

            for(int i = 0; i < n_categories; i++)
            {
                file.read(memblock, 4);
                priority_sorted_category_ids.push_back(FileUtils::readInt32((unsigned char*) memblock));
            }

            delete [] memblock;
        }

        file.close();
        return true;
    }

    return false;
}

void AnalysisConfiguration::write(std::string filename) const
{
    std::ofstream file;
    file.open(filename, std::ios_base::binary | std::ios_base::trunc );

    // First write the header
    std::string header (CONFIGURATION_SIGNATURE);
    file.write(header.c_str(), CONFIGURATION_SIGNATURE_LENGTH);
    file.write((char*) FileUtils::toBin((unsigned int)r_min,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)r_max,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)r_diff,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)analysis_window_width,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)analysis_window_height,4),4);

    // Sorted category ids
    file.write((char*) FileUtils::toBin((unsigned int)priority_sorted_category_ids.size(),4),4);
    for(int category_id : priority_sorted_category_ids)
        file.write((char*) FileUtils::toBin((unsigned int)category_id,4),4);

    file.close();
}

void AnalysisConfiguration::writeToCSV(std::string filename) const
{
    std::ofstream file;
    file.open(filename);

    if(file.is_open())
    {
        // First write the header
        file << "R Min," << r_min << "\n";
        file << "R Max," << r_max << "\n";
        file << "R Diff," << r_diff << "\n";
        file << "Analysis window width," << analysis_window_width << "\n";
        file << "Analysis window height," << analysis_window_width << "\n";
        file << "Priortity sorted category ids,";
        for(int category_id : priority_sorted_category_ids)
            file << category_id << ",";
        file << "\n";
    }
    file.close();
}

void AnalysisConfiguration::setPrioritySortedCategoryIds(std::vector<int> priority_sorted_category_ids)
{
    this->priority_sorted_category_ids = priority_sorted_category_ids;
}
