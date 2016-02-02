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
    CategoryPropertiesHeader(int category_id, int priority, int n_points, int bin_size, std::pair<int,int> radius_range, std::pair<int,int> height_range,
                             std::pair<int,int> root_size_range, float radius_avg, float height_avg, float root_size_avg) :
        category_id(category_id), priority(priority), n_points(n_points), bin_size(bin_size), radius_range(radius_range),
        height_range(height_range), root_size_range(root_size_range), radius_avg(radius_avg), height_avg(height_avg), root_size_avg(root_size_avg)
    {}

    bool operator ==(const CategoryPropertiesHeader &other) const
    {
        return (category_id == other.category_id &&
                radius_range == other.radius_range &&
                height_range == other.height_range &&
                root_size_range == other.root_size_range &&
                radius_avg == other.radius_avg &&
                height_avg == other.height_avg &&
                root_size_avg == other.root_size_avg &&
                bin_size == other.bin_size &&
                n_points == other.n_points &&
                priority == other.priority &&
                category_dependent_ids == other.category_dependent_ids);
    }

    int category_id;
    int bin_size;
    int n_points;
    int priority;
    std::set<int> category_dependent_ids;

    std::pair<int,int> radius_range;
    float radius_avg;

    std::pair<int,int> height_range;
    float height_avg;

    std::pair<int,int> root_size_range;
    float root_size_avg;
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
