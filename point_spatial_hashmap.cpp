#include "point_spatial_hashmap.h"
#include "analysis_point.h"
#include <QLineF>

#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
PointSpatialHashmap::PointSpatialHashmap(int width, int height) :
    SpatialHashMap<PointSpatialHashMapCell>(std::ceil(width/100.f), std::ceil(height/100.f),
                                            std::ceil(((float)width)/std::ceil(width/100.f)),
                                            std::ceil(((float)height)/std::ceil(height/100.f)))
{

}

PointSpatialHashmap::~PointSpatialHashmap()
{

}

void PointSpatialHashmap::addPoint(const AnalysisPoint & p)
{
    std::vector<QPoint> cells(SpatialHashMap<PointSpatialHashMapCell>::getPoints(p.getCenter(), p.getRadius(), false));

    for(QPoint & c : cells)
    {
        this->getCell(c, PointSpatialHashmap::Space::_HASHMAP).points.insert(p);
    }
}

void PointSpatialHashmap::removePoint(const AnalysisPoint & p)
{
    std::vector<QPoint> cells(SpatialHashMap<PointSpatialHashMapCell>::getPoints(p.getCenter(), p.getRadius(), false));

    for(QPoint & c : cells)
    {
        this->getCell(c, PointSpatialHashmap::Space::_HASHMAP).points.erase(p);
    }
}

std::vector<QPoint> PointSpatialHashmap::get_points(const AnalysisPoint & p, int r_max)
{
    QPoint center(p.getCenter());
    BoundingBox min_bb, max_bb;
    if(p.getRadius() > 0)
    {
        // MIN
        {
            QPoint min(center.x()-p.getRadius(), center.y()-p.getRadius());
            min_bb = SpatialHashMap::get_bounding_box(min, r_max);
        }
        // MAX
        {
            QPoint max(center.x()+p.getRadius(), center.y()+p.getRadius());
            max_bb = SpatialHashMap::get_bounding_box(max, r_max);
        }
    }
    else
        min_bb = max_bb = SpatialHashMap::get_bounding_box(p.getCenter(), r_max);


    std::vector<QPoint> ret;

    for(int x (min_bb.min.x()); x <= max_bb.max.x(); x++)
    {
        for(int y (min_bb.min.y()); y <= max_bb.max.y(); y++)
        {
            ret.push_back(QPoint(x,y));
        }
    }

    return ret;
}

bool PointSpatialHashmap::coversMultipleCells(const AnalysisPoint & p)
{
    return SpatialHashMap::coversMultipleCells(p.getCenter(), p.getRadius());
}
#include <iostream>
std::vector<AnalysisPoint> PointSpatialHashmap::getPossibleReachablePoints(const AnalysisPoint & reference_point, int r_max)
{
    std::vector<AnalysisPoint> ret;

    std::unordered_map<AnalysisPoint, bool> processed_multicell_target_points;

    std::vector<QPoint> cells_within_r_max( get_points(reference_point, r_max) );

    for(QPoint & c : cells_within_r_max)
    {
        if(initialised(c)) // If not initialised, no points to process
        {
            PointSpatialHashMapCell & cell (getCell(c, PointSpatialHashmap::Space::_HASHMAP));

            for(const AnalysisPoint & destination_point : cell.points)
            {
                if(reference_point != destination_point)
                {
                    if(!destination_point.multiCellCoverageTestPerformed())
                        destination_point.setCoversMultipleCells(coversMultipleCells(destination_point));
                    bool multi_cell_target_point(destination_point.coversMultipleCells());
                    if(!multi_cell_target_point ||
                            (processed_multicell_target_points.find(destination_point) == processed_multicell_target_points.end() ))
                    {
                        if(multi_cell_target_point)
                            processed_multicell_target_points[destination_point] = true;
                        ret.push_back(destination_point);
                    }
                }
            }
        }
    }

    return ret;
}
