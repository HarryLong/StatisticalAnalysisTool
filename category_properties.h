#ifndef CATEGORY_PROPERTIES_H
#define CATEGORY_PROPERTIES_H

#include <map>
#include <string>
#include <vector>

#define CATEGORY_PROPERTIES_SIGNATURE "POINTPROPERTIESFILE"
#define CATEGORY_PROPERTIES_SIGNATURE_LENGTH 19
struct CategoryPropertiesHeader{
public:
    CategoryPropertiesHeader() {}
    CategoryPropertiesHeader(int category_id, int priority, int n_points, int min_size, int max_size, int bin_size) :
        category_id(category_id), priority(priority), n_points(n_points), min_size(min_size), max_size(max_size), bin_size(bin_size)
    {}

    int category_id;
    int min_size;
    int max_size;
    int bin_size;
    int n_points;
    int priority;
    std::vector<int> category_dependent_ids;
};

class CategoryProperties
{
public:
    typedef std::map<int,float> Histogram;

    CategoryProperties(CategoryPropertiesHeader header, Histogram data);
    CategoryProperties(std::string filename);
    void write(std::string filename);
    void writeToCSV(std::string filename);

    CategoryPropertiesHeader m_header;
    Histogram m_data;
private:
    bool load_data(std::string filename);
};

#endif // CATEGORY_PROPERTIES_H