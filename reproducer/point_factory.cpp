#include "point_factory.h"
#include "../analysis_point.h"
#include <iostream>
#include <QDebug>

PointFactory::PointFactory(int width, int height) :
    m_dice_roller(0,RAND_MAX), m_taken_points(),
    m_width(width),
    m_height(height)
{

}

PointFactory::~PointFactory()
{

}

void PointFactory::setCategoryProperties(CategoryProperties properties)
{
    m_active_cp = properties;
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
//    m_point_size_diffs = properties.m_header.bin_size;
//    m_active_category_id = properties.m_header.category_id;
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
    int random_number(m_dice_roller.generate()%1000);
    auto it(m_probability_to_bin_size_pairs.begin());
    for(; it != m_probability_to_bin_size_pairs.end(); it++)
    {
        if(random_number < it->first)
            return getPoint(it->second + (m_dice_roller.generate()%m_active_cp.m_header.bin_size));
    }

    qCritical() << "Bug!";
    return AnalysisPoint();
}

AnalysisPoint PointFactory::getPoint(int height)
{
    if(m_active_cp.m_header.category_id == -1)
    {
        std::cerr << "Attempting to get point from Point Factory with no active category id" << std::endl;
        exit(1);
    }

    QPoint position;
    do{
        position = QPoint(m_dice_roller.generate()%(m_width-1),
                          m_dice_roller.generate()%(m_height-1));
    }while(m_taken_points.containsPoint(position));


//    qCritical() << "--------------------------------------";
//    qCritical() << "Category id: " << m_active_cp.m_header.category_id;
//    qCritical() << "Height: " << height;
//    qCritical() << "Canopy r: " << height*m_active_cp.m_header.height_to_radius_multiplier;
//    qCritical() << "Root size: " << height*m_active_cp.m_header.height_to_root_size_multiplier;

    return AnalysisPoint(m_active_cp.m_header.category_id, position, height*m_active_cp.m_header.height_to_radius_multiplier,
                         height*m_active_cp.m_header.height_to_root_size_multiplier, height);

//    AnalysisPoint(int category_id, QPoint center, int canopy_radius, int root_width, int height);

}

int PointFactory::getActiveCategoryId()
{
    return m_active_cp.m_header.category_id;
}

