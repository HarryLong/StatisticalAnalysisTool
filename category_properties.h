#ifndef CATEGORY_PROPERTIES_H
#define CATEGORY_PROPERTIES_H

#include <map>
#include <string>
#include <vector>
#include <set>

#define CATEGORY_PROPERTIES_SIGNATURE "CATEGORYPROPERTIESFILE"
#define CATEGORY_PROPERTIES_SIGNATURE_LENGTH 22
struct CategoryPropertiesHeader{
public:
    CategoryPropertiesHeader() {}
    CategoryPropertiesHeader(int category_id, int priority, int n_points, int min_size, int max_size, int bin_size) :
        category_id(category_id), priority(priority), n_points(n_points), min_size(min_size), max_size(max_size), bin_size(bin_size)
    {}

    bool operator ==(const CategoryPropertiesHeader &other) const
    {
        return (category_id == other.category_id &&
                min_size == other.min_size &&
                max_size == other.max_size &&
                bin_size == other.bin_size &&
                n_points == other.n_points &&
                priority == other.priority &&
                category_dependent_ids == other.category_dependent_ids);
    }

    int category_id;
    int min_size;
    int max_size;
    int bin_size;
    int n_points;
    int priority;
    std::set<int> category_dependent_ids;
};

class CategoryProperties
{
public:
    typedef std::map<int,float> Histogram;

    CategoryProperties(CategoryPropertiesHeader header, Histogram data);
    CategoryProperties(std::string filename);
    CategoryProperties();
    void write(std::string filename);
    void writeToCSV(std::string filename);

    CategoryPropertiesHeader m_header;
    Histogram m_data;
private:
    bool load_data(std::string filename);
};

#endif // CATEGORY_PROPERTIES_H
