#ifndef ANALYSIS_POINT_H
#define ANALYSIS_POINT_H

#include <QPoint>

class AnalysisPoint{
public:
    AnalysisPoint(int category_id, QPoint center, int radius = 1);
    virtual ~AnalysisPoint();

    int getRadius() const;
    QPoint getCenter() const;

    void setCenter(QPoint center);
    void setRadius(int radius);
    int getCategoryId() const;

private:
    int r;
    QPoint center;
    int category_id;
};

#endif // ANALYSIS_POINT_H
