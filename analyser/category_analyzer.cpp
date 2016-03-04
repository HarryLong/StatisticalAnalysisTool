#include "category_analyzer.h"
#include "../analysis_point.h"

#include <climits>
#include <thread>

CategoryAnalyzer::CategoryAnalyzer(std::vector<AnalysisPoint> & points, int category_id, int priority,
                                   CompletionListener * completion_listener, int bin_size) :
    m_points(points), m_category_id(category_id), m_priority(priority), m_completion_listener(completion_listener),
    m_bin_size(bin_size)
{

}

CategoryAnalyzer::~CategoryAnalyzer()
{
}
void CategoryAnalyzer::calculateCategoryProperties(bool asynchronous)
{
    if(asynchronous)
        std::thread(&CategoryAnalyzer::calculate_category_properties, this).detach();
    else
        calculate_category_properties();
}

#include <QDebug>
void CategoryAnalyzer::calculate_category_properties()
{
    CategoryProperties::Histogram data;

    SizeProperties radius_properties, height_properties, root_size_properties;

//    int radius_min(-1);
//    int radius_max(-1);
//    float radius_avg(.0f);

//    int height_min(-1);
//    int height_max(-1);
//    float height_avg(.0f);

//    int root_size_min(-1);
//    int root_size_max(-1);
//    float root_size_avg(.0f);

    float addition_value(1.0f/m_points.size());

    for(AnalysisPoint & p : m_points)
    {
        int radius(p.getRadius());
        radius_properties.avg += radius;

        int height(p.getHeight());
        height_properties.avg += height;

        int root_size(p.getRootWidth());
        root_size_properties.avg += root_size;

        int bracket(CategoryAnalyzer::getBin(p.getHeight(), m_bin_size));

        // Radius --> Used for statistical analysis
        if(radius_properties.min == -1 || radius < radius_properties.min)
            radius_properties.min = radius;
        if (radius_properties.max == -1 || radius > radius_properties.max)
            radius_properties.max = radius;

        // Height
        if(height_properties.min == -1 || height < height_properties.min)
            height_properties.min = height;
        if (height_properties.max == -1 || height > height_properties.max)
            height_properties.max = height;

        // Root size
        if(root_size_properties.min == -1 || root_size < root_size_properties.min)
            root_size_properties.min = root_size;
        if (root_size_properties.max == -1 || root_size > root_size_properties.max)
            root_size_properties.max = root_size;

        if(data.find(bracket) == data.end())
            data.insert(std::pair<int,float>(bracket, .0f));

        data[bracket] += addition_value;
    }

    // Canopy width
    if(radius_properties.min == -1)
        radius_properties.min = 0;
    if(radius_properties.max == -1)
        radius_properties.max = 0;

    // Height
    if(height_properties.min == -1)
        height_properties.min = 0;
    if(height_properties.max == -1)
        height_properties.max = 0;

    // Root size
    if(root_size_properties.min == -1)
        root_size_properties.min = 0;
    if(root_size_properties.max == -1)
        root_size_properties.max = 0;

    // Averages
    if(m_points.size() > 0)
        radius_properties.avg /= m_points.size();
    height_properties.avg = std::max(.0f, height_properties.avg /= m_points.size());
    root_size_properties.avg = std::max(.0f, root_size_properties.avg /= m_points.size());

    float height_to_canopy_radius(std::max(.0f, ((float)radius_properties.max)/height_properties.max));
    float height_to_root_size(std::max(.0f, ((float)root_size_properties.max)/height_properties.max));

    CategoryPropertiesHeader header(m_category_id, m_priority, m_points.size(), m_bin_size, height_to_canopy_radius, height_to_root_size,
                                    radius_properties, height_properties, root_size_properties);

    m_category_properties = CategoryProperties(header, data);

    if(m_completion_listener)
        m_completion_listener->complete(m_category_properties);
}

CategoryProperties CategoryAnalyzer::getCategoryProperties()
{
    return m_category_properties;
}

int CategoryAnalyzer::getBin(int size, int diff)
{
    return ( size/diff ) * diff;
}
