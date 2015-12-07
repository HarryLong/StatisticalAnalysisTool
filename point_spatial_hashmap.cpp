#include "point_spatial_hashmap.h"
#include "analysis_point.h"
#include <QLineF>

#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
PointSpatialHashmap::PointSpatialHashmap(int width, int height) :
    SpatialHashMap<PointSpatialHashMapCell>(SPATIAL_HASHMAP_CELL_WIDTH, SPATIAL_HASHMAP_CELL_HEIGHT,
                                            std::ceil(((float)width)/SPATIAL_HASHMAP_CELL_WIDTH),
                                            std::ceil(((float)height)/SPATIAL_HASHMAP_CELL_HEIGHT))
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
        this->operator [](c).points.insert(p);
    }
}

void PointSpatialHashmap::removePoint(const AnalysisPoint & p)
{
    std::vector<QPoint> cells(SpatialHashMap<PointSpatialHashMapCell>::getPoints(p.getCenter(), p.getRadius(), false));

    for(QPoint & c : cells)
    {
        this->operator [](c).points.erase(p);
    }
}

std::vector<QPoint> PointSpatialHashmap::get_points(const AnalysisPoint & p, int r_max)
{
    return SpatialHashMap<PointSpatialHashMapCell>::getPoints(p.getCenter(), r_max+p.getRadius(), false);
}

bool PointSpatialHashmap::coversMultipleCells(const AnalysisPoint & p)
{
    return SpatialHashMap::coversMultipleCells(p.getCenter(), p.getRadius());
}

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

//            cell.points.insert(AnalysisPoint());

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
                        ret.push_back(destination_point);

                        if(multi_cell_target_point)
                            processed_multicell_target_points[destination_point] = true;
                    }
                }
            }
        }
    }

    return ret;
}
