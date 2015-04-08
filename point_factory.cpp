#include "point_factory.h"
#include "analysis_point.h"
#include <iostream>

PointFactory::PointFactory(int max_width, int max_height) :
    m_dice_roller(0,RAND_MAX), m_taken_points(),m_active_category_id(-1),
    m_max_width(max_width),
    m_max_height(max_height)
{

}

PointFactory::~PointFactory()
{

}

void PointFactory::setPointSizeProperties(PointSizeProperties properties)
{
    m_point_percentages.clear();
    auto it(properties.m_data.begin());
    for(int p(0); p < 1000;it++)
    {
        int r(it->first);
        float ratio_over_thousand(it->second*1000);
        for(int i(0); i < ratio_over_thousand; i++)
            m_point_percentages[p+i] = r;

        p += ratio_over_thousand;
    }
    m_point_size_diffs = properties.m_header.diff;
    m_active_category_id = properties.m_header.points_id;
}

void PointFactory::setPositionStatus(QPoint point, bool available)
{
    if(available)
        m_taken_points.removePoint(point);
    else
        m_taken_points.insertPoint(point);
}

AnalysisPoint* PointFactory::getPoint()
{
    if(m_active_category_id == -1)
    {
        std::cerr << "Attempting to get point from Point Factory with no active category id" << std::endl;
        exit(1);
    }

    QPoint position;
    do{
        position = QPoint(m_dice_roller.generate()%m_max_width,
                          m_dice_roller.generate()%m_max_height);
    }while(m_taken_points.containsPoint(position));

    AnalysisPoint * ret (new AnalysisPoint(m_active_category_id, position, m_point_percentages[m_dice_roller.generate()%1000] + (m_dice_roller.generate()%m_point_size_diffs)));

    return ret;
}

int PointFactory::getActiveCategoryId()
{
    return m_active_category_id;
}

