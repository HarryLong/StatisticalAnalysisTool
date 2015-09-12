#ifndef CATEGORY_ANALYZER_H
#define CATEGORY_ANALYZER_H

#include "../category_properties.h"
#include <vector>

class AnalysisPoint;
class CategoryAnalyzer{
public:
    class CompletionListener{
    public:
        virtual void complete(CategoryProperties & category_properties) = 0;
    };

    CategoryAnalyzer(std::vector<AnalysisPoint>& points, int category_id, int priority,
                     CompletionListener * completion_listener = NULL, int bin_size = 1);
    ~CategoryAnalyzer();
    void calculateCategoryProperties(bool asynchronous = true);
    CategoryProperties getCategoryProperties();

    static int getBin(int size, int diff);
private:
    void calculate_category_properties();
    int m_bin_size;
    std::vector<AnalysisPoint> m_points;
    int m_category_id;
    int m_priority;
    CategoryProperties m_category_properties;
    CompletionListener * m_completion_listener;
};

#endif //CATEGORY_ANALYZER_H
