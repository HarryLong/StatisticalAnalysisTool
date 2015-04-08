#include "point_properties.h"
#include <vector>

class AnalysisPoint;
class PointSizeAnalyzer{
public:
    static PointSizeProperties getSizeProperties(std::vector<AnalysisPoint*> points, int diff, int points_id = 0);
    static int getSizeBracket(int size, int diff);
};
