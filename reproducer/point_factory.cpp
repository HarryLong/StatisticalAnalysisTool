#include "point_factory.h"
#include "../analysis_point.h"
#include <iostream>

PointFactory::PointFactory(int width, int height) :
    m_dice_roller(0,RAND_MAX), m_taken_points(),m_active_category_id(-1),
    m_width(width),
    m_height(height)
{

}

PointFactory::~PointFactory()
{

}

void PointFactory::setCategoryProperties(CategoryProperties properties)
{
    int current_percentage(0);
    m_probability_to_bin_size_pairs.clear();

    unsigned int i(0);
    for(std::pair<int,float> size_percentage_pair : properties.m_data)
    {
        if(size_percentage_pair.second > 0)
        {
            if(++i == properties.m_data.size())
                current_percentage = 1000;
            else
                current_percentage += (size_percentage_pair.second*1000);
            m_probability_to_bin_size_pairs.insert(std::pair<int,int>(current_percentage, size_percentage_pair.first));
        }
    }
    m_point_size_diffs = properties.m_header.bin_size;
    m_active_category_id = properties.m_header.category_id;
}

void PointFactory::setPositionStatus(QPoint point, bool available)
{
    if(available)
        m_taken_points.removePoint(point);
    else
        m_taken_points.insertPoint(point);
}

AnalysisPoint PointFactory::getPoint()
{
    if(m_active_category_id == -1)
    {
        std::cerr << "Attempting to get point from Point Factory with no active category id" << std::endl;
        exit(1);
    }

    QPoint position;
    do{
        position = QPoint(m_dice_roller.generate()%(m_width-1),
                          m_dice_roller.generate()%(m_height-1));
    }while(m_taken_points.containsPoint(position));

    int size;
    {
        int random_number(m_dice_roller.generate()%1000);
        auto it(m_probability_to_bin_size_pairs.begin());
        for(; it != m_probability_to_bin_size_pairs.end(); it++)
        {
            if(random_number < it->first)
                break;
        }
        size = (it->second + (m_dice_roller.generate()%m_point_size_diffs));
    }

    return AnalysisPoint(m_active_category_id, position, size);
}

int PointFactory::getActiveCategoryId()
{
    return m_active_category_id;
}

