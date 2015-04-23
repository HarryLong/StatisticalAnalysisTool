#include "category_properties.h"
#include <vector>

class AnalysisPoint;
class CategoryAnalyzer{
public:
    CategoryAnalyzer(int bin_size = 1);
    ~CategoryAnalyzer();
    CategoryProperties getCategoryProperties(std::vector<AnalysisPoint*>& points, int category_id, int priority);
    static int getBin(int size, int diff);
private:
    int m_bin_size;
};
