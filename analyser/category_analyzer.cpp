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

void CategoryAnalyzer::calculate_category_properties()
{
    CategoryProperties::Histogram data;
    int radius_min(-1);
    int radius_max(-1);
    float radius_avg(.0f);

    int height_min(-1);
    int height_max(-1);
    float height_avg(.0f);

    int root_size_min(-1);
    int root_size_max(-1);
    float root_size_avg(.0f);

    float addition_value(1.0f/m_points.size());

    for(AnalysisPoint & p : m_points)
    {
        int radius(p.getRadius());
        radius_avg+= radius;

        int height(p.getHeight());
        height_avg += height;

        int root_size(p.getRootWidth());
        root_size_avg += root_size;

        int bracket(CategoryAnalyzer::getBin(p.getRadius(), m_bin_size));

        // Radius --> Used for statistical analysis
        if(radius_min == -1 || radius < radius_min)
            radius_min = radius;
        if (radius_max == -1 || radius > radius_max)
            radius_max = radius;

        // Height
        if(height_min == -1 || height < height_min)
            height_min = height;
        if (height_max == -1 || height > height_max)
            height_max = height;

        // Root size
        if(root_size_min == -1 || root_size < root_size_min)
            root_size_min = root_size;
        if (root_size_max == -1 || root_size > root_size_max)
            root_size_max = root_size;

        if(data.find(bracket) == data.end())
            data.insert(std::pair<int,float>(bracket, .0f));

        data[bracket] += addition_value;
    }

    // Averages
    radius_avg /= m_points.size();
    if(height_avg != -1)
        height_avg /= m_points.size();
    if(root_size_avg != -1)
        root_size_avg /= m_points.size();

    CategoryPropertiesHeader header(m_category_id, m_priority, m_points.size(), m_bin_size, std::pair<int,int>(radius_min, radius_max),
                                    std::pair<int,int>(height_min, height_max), std::pair<int,int>(root_size_min, root_size_max),
                                    radius_avg, height_avg, root_size_avg);

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
