#include "category_analyzer.h"
#include "analysis_point.h"
#include <climits>

CategoryAnalyzer::CategoryAnalyzer(int bin_size) : m_bin_size(bin_size)
{

}

CategoryAnalyzer::~CategoryAnalyzer()
{

}

CategoryProperties CategoryAnalyzer::getCategoryProperties(std::vector<AnalysisPoint *>& points, int category_id, int priority)
{
    CategoryProperties::Histogram data;
    int min(INT_MAX);
    int max(-1);

    float addition_value(1.0f/points.size());

    for(AnalysisPoint* p : points)
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

    CategoryPropertiesHeader header(category_id, priority, points.size(), min, max, m_bin_size);

    return CategoryProperties(header, data);
}

int CategoryAnalyzer::getBin(int size, int diff)
{
    return ( size/diff ) * diff;
}
