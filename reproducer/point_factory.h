#ifndef POINT_FACTORY_H
#define POINT_FACTORY_H

#include "../category_properties.h"
#include "../utils/dice_roller.h"
#include "../point_map.h"

#include <map>
#include <QPoint>

class AnalysisPoint;

class PointFactory
{
public:
    PointFactory(int width, int height);
    ~PointFactory();

    AnalysisPoint getPoint(int height);
    AnalysisPoint getPoint();
    void setPositionStatus(QPoint point, bool available);
    void setCategoryProperties(CategoryProperties properties);
    int getActiveCategoryId();

private:
    DiceRoller m_dice_roller;
    int m_width;
    int m_height;
//    int m_point_size_diffs;
//    int m_active_category_id;
    std::map<int,int> m_probability_to_bin_size_pairs;
    PointMap m_taken_points;
    CategoryProperties m_active_cp;
};


#endif //POINT_FACTORY_H
