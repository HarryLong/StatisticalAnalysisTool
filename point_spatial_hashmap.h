#include "spatial_hashmap.h"

class AnalysisPoint;
class PointSpatialHashMapCell {
public:
    PointSpatialHashMapCell() : points() { }
    ~PointSpatialHashMapCell() {}
    std::vector<AnalysisPoint*> points;
};

class PointSpatialHashmap : public SpatialHashMap<PointSpatialHashMapCell>
{
public:
    PointSpatialHashmap(int width, int height);
    ~PointSpatialHashmap();

    std::vector<PointSpatialHashMapCell*> getCells(const AnalysisPoint * p, int r_max);
    void removePoint(AnalysisPoint * p);
    void addPoint(AnalysisPoint * p);
    bool coversMultipleCells(AnalysisPoint * p);
    std::vector<AnalysisPoint*> getPossibleReachablePoints(const AnalysisPoint * reference_point, int r_max);
};
