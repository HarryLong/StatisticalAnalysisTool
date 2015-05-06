#include "category_analyzer.h"
#include "analysis_point.h"
#include <climits>
#include <thread>

CategoryAnalyzer::CategoryAnalyzer(std::vector<AnalysisPoint *>& points, int category_id, int priority,
                                   CategoryAnalysisCompletionListener * completion_listener, int bin_size) :
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
    int min(INT_MAX);
    int max(-1);

    float addition_value(1.0f/m_points.size());

    for(AnalysisPoint* p : m_points)
    {
        int size(p->getRadius());
        int bracket(CategoryAnalyzer::getBin(p->getRadius(), m_bin_size));
        if(size < min)
            min = size;
        if (size > max)
            max = size;

        if(data.find(bracket) == data.end())
            data.insert(std::pair<int,float>(bracket, .0f));

        data[bracket] += addition_value;
    }

    CategoryPropertiesHeader header(m_category_id, m_priority, m_points.size(), min, max, m_bin_size);

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
