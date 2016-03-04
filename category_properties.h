#ifndef CATEGORY_PROPERTIES_H
#define CATEGORY_PROPERTIES_H

#include <map>
#include <string>
#include <vector>
#include <set>
#include "size_properties.h"

#define CATEGORY_PROPERTIES_SIGNATURE "CATEGORYPROPERTIESFILE"
#define CATEGORY_PROPERTIES_SIGNATURE_LENGTH 22
struct CategoryPropertiesHeader{
public:
    CategoryPropertiesHeader() {}
    CategoryPropertiesHeader(int category_id, int priority, int n_points, int bin_size, float height_to_radius_multiplier, float height_to_root_size_multiplier,
                             SizeProperties radius_properties, SizeProperties height_properties, SizeProperties root_size_properties) :
        category_id(category_id), priority(priority), n_points(n_points), bin_size(bin_size), height_to_radius_multiplier(height_to_radius_multiplier),
        height_to_root_size_multiplier(height_to_root_size_multiplier), radius_properties(radius_properties), height_properties(height_properties),
        root_size_properties(root_size_properties)
    {}

    bool operator ==(const CategoryPropertiesHeader &other) const
    {
        return (category_id == other.category_id &&
                radius_properties == other.radius_properties &&
                height_properties == other.height_properties &&
                root_size_properties == other.root_size_properties &&
                bin_size == other.bin_size &&
                n_points == other.n_points &&
                priority == other.priority &&
                height_to_radius_multiplier == other.height_to_radius_multiplier &&
                height_to_root_size_multiplier == other.height_to_root_size_multiplier &&
                category_dependent_ids == other.category_dependent_ids);
    }

    int category_id;
    int bin_size;
    int n_points;
    int priority;
    float height_to_radius_multiplier;
    float height_to_root_size_multiplier;
    std::set<int> category_dependent_ids;

    SizeProperties radius_properties, height_properties, root_size_properties;
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
