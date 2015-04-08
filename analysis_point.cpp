#include "analysis_point.h"

AnalysisPoint::AnalysisPoint(int category_id, QPoint center, int radius) :
    center(center), category_id(category_id), r(radius)
{

}

AnalysisPoint::~AnalysisPoint()
{

}

int AnalysisPoint::getRadius() const
{
    return r;
}

QPoint AnalysisPoint::getCenter() const
{
    return center;
}

void AnalysisPoint::setCenter(QPoint center)
{
    this->center = center;
}

void AnalysisPoint::setRadius(int radius)
{
    this->r = radius;
}

int AnalysisPoint::getCategoryId() const
{
    return category_id;
}
