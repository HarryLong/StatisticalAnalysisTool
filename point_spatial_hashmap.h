#include "SpatialHashmap/spatial_hashmap.h"
#include <unordered_set>
#include "analysis_point.h"

class PointSpatialHashMapCell {
public:
    PointSpatialHashMapCell() : points() { }
    ~PointSpatialHashMapCell() {}
    std::unordered_set<AnalysisPoint> points;
};

class PointSpatialHashmap : public SpatialHashMap<PointSpatialHashMapCell>
{
public:
    PointSpatialHashmap(int width, int height);
    ~PointSpatialHashmap();

    void removePoint(const AnalysisPoint & p);
    void addPoint(const AnalysisPoint & p);
    bool coversMultipleCells(const AnalysisPoint & p);
    std::vector<AnalysisPoint> getPossibleReachablePoints(const AnalysisPoint & reference_point, int r_max);

private:
    std::vector<QPoint> get_points(const AnalysisPoint & p, int r_max);

};
