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
    // Initialize all the cells
    for(int x = 0; x < getHorizontalCellCount(); x++)
    {
        for(int y = 0; y < getVerticalCellCount(); y++)
        {
            QPoint cell_location(x,y);
            if(!get(cell_location))
                init_cell(cell_location);
        }
    }
}

PointSpatialHashmap::~PointSpatialHashmap()
{

}

void PointSpatialHashmap::addPoint(AnalysisPoint * p)
{
    for(PointSpatialHashMapCell* cell : SpatialHashMap<PointSpatialHashMapCell>::getCells(p->getCenter(), p->getRadius(), false))
    {
        cell->points.push_back(p);
    }
}

void PointSpatialHashmap::removePoint(AnalysisPoint * p)
{
    for(PointSpatialHashMapCell* cell : SpatialHashMap<PointSpatialHashMapCell>::getCells(p->getCenter(), p->getRadius(), false))
    {
        cell->points.erase(std::find(cell->points.begin(), cell->points.end(), p));
    }
}

std::vector<PointSpatialHashMapCell*> PointSpatialHashmap::getCells(const AnalysisPoint * p, int r_max)
{
    return SpatialHashMap<PointSpatialHashMapCell>::getCells(p->getCenter(), r_max+p->getRadius(), false);
}

bool PointSpatialHashmap::coversMultipleCells(AnalysisPoint * p)
{
    return SpatialHashMap::coversMultipleCells(p->getCenter(), p->getRadius());
}

std::vector<AnalysisPoint*> PointSpatialHashmap::getPossibleReachablePoints(const AnalysisPoint * reference_point, int r_max)
{
    std::vector<AnalysisPoint*> ret;

    std::unordered_map<AnalysisPoint*, bool> processed_multicell_target_points;

    std::vector<PointSpatialHashMapCell*> cells_within_r_max( getCells(reference_point, r_max) );

    for(PointSpatialHashMapCell * cell : cells_within_r_max)
    {
        for(AnalysisPoint* destination_point : cell->points)
        {
            if(reference_point != destination_point)
            {
                bool multi_cell_target_point(coversMultipleCells(destination_point));
                if(!multi_cell_target_point ||
                        (processed_multicell_target_points.find(destination_point) == processed_multicell_target_points.end() ))
                {
                    ret.push_back(destination_point);

                    if(multi_cell_target_point)
                        processed_multicell_target_points[destination_point] = true;                }
            }
        }
    }

    return ret;
}
