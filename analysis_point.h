#ifndef ANALYSIS_POINT_H
#define ANALYSIS_POINT_H

#include <QPoint>

class AnalysisPoint{
public:
    AnalysisPoint(int category_id, QPoint center, int radius = 1);
    virtual ~AnalysisPoint();

    virtual int getRadius() const;
    virtual QPoint getCenter() const;
    virtual int getCategoryId() const;

    void setCenter(QPoint center);
    void setRadius(int radius);

private:
    int r;
    QPoint center;
    int category_id;
};

#endif // ANALYSIS_POINT_H
