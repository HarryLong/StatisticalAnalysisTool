#ifndef ANALYSIS_CONFIGURATION_H
#define ANALYSIS_CONFIGURATION_H

#include <vector>
#include <string>

#define CONFIGURATION_SIGNATURE "CONFIGURATIONFILE"
#define CONFIGURATION_SIGNATURE_LENGTH 17
class AnalysisConfiguration{
public:
    AnalysisConfiguration(int r_min, int r_max, int r_diff, int analysis_window_width, int analysis_window_height, std::vector<int> priority_sorted_category_ids =
            std::vector<int>());
    AnalysisConfiguration(std::string filename);

    bool load_data(std::string filename);
    void write(std::string filename) const;
    void writeToCSV(std::string filename) const;
    void setPrioritySortedCategoryIds(std::vector<int> priority_sorted_category_ids);

    int r_min;
    int r_max;
    int r_diff;
    int analysis_window_width;
    int analysis_window_height;
    std::vector<int> priority_sorted_category_ids;

    bool operator ==(const AnalysisConfiguration &other) const;
};

#endif //ANALYSIS_CONFIGURATION_H
