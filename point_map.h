#ifndef POINT_MAP_H
#define POINT_MAP_H

#include <map>
#include <QPoint>

class PointMap : protected std::map<std::pair<int,int>,int>
{
public:
    PointMap();
    void insertPoint(QPoint p);
    void removePoint(QPoint p);
    bool containsPoint(QPoint p);
    std::map<std::pair<int,int>,int>::iterator begin();
    std::map<std::pair<int,int>,int>::iterator end();
};

#endif // POINT_MAP_H
