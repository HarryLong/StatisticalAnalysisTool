#ifndef POINT_FACTORY_H
#define POINT_FACTORY_H

#include <map>
#include <QPoint>
#include "point_properties.h"
#include "dice_roller.h"
#include "point_map.h"

class AnalysisPoint;

class PointFactory
{
public:
    PointFactory(int max_width, int max_height);
    ~PointFactory();

    AnalysisPoint* getPoint();
    void setPositionStatus(QPoint point, bool available);
    void setPointSizeProperties(PointSizeProperties properties);
    int getActiveCategoryId();

private:
    DiceRoller m_dice_roller;
    std::map<int, int> m_point_percentages;
    int m_max_width;
    int m_max_height;
    int m_point_size_diffs;
    int m_active_category_id;
    PointMap m_taken_points;
};


#endif //POINT_FACTORY_H
