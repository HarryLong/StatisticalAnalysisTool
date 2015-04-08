#include "point_analyzer.h"
#include "analysis_point.h"
#include <climits>

PointSizeProperties PointSizeAnalyzer::getSizeProperties(std::vector<AnalysisPoint *> points, int diff, int points_id)
{
    PointSizeProperties::Histogram data;
    int min(INT_MAX);
    int max(-1);

    float addition_value(1.0f/points.size());

    for(AnalysisPoint* p : points)
    {
        int size(p->getRadius());
        int bracket(PointSizeAnalyzer::getSizeBracket(p->getRadius(), diff));
        if(size < min)
            min = size;
        if (size > max)
            max = size;

        if(data.find(bracket) == data.end())
            data.insert(std::pair<int,float>(bracket, .0f));

        data[bracket] += addition_value;
    }

    PointSizePropertiesHeader header(points_id, points.size(), min, max, diff);

    return PointSizeProperties(header, data);
}

int PointSizeAnalyzer::getSizeBracket(int size, int diff)
{
    return ( size/diff ) * diff;
}
