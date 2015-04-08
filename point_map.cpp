#include "point_map.h"

PointMap::PointMap()
{

}

void PointMap::insertPoint(QPoint p)
{
    insert(std::pair<std::pair<int,int>, int>(std::pair<int,int>(p.x(),p.y()), true));
}

void PointMap::removePoint(QPoint p)
{
    erase(std::pair<int,int>(p.x(),p.y()));
}

bool PointMap::containsPoint(QPoint p)
{
    return find(std::pair<int,int>(p.x(),p.y())) != std::map<std::pair<int,int>,int>::end();
}

std::map<std::pair<int,int>,int>::iterator PointMap::begin()
{
    return std::map<std::pair<int,int>,int>::begin();
}

std::map<std::pair<int,int>,int>::iterator PointMap::end()
{
    return std::map<std::pair<int,int>,int>::end();
}
