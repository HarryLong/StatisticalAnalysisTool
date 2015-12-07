#include "analysis_point.h"

AnalysisPoint::AnalysisPoint(int category_id, QPoint center, int radius) :
    center(center), category_id(category_id), r(radius), covers_multiple_cells(false), milticell_coverage_test_performed(false)
{

}

AnalysisPoint::AnalysisPoint() : AnalysisPoint(-1, QPoint(-1,-1), -1)
{

}

AnalysisPoint::~AnalysisPoint()
{

}

bool AnalysisPoint::coversMultipleCells() const
{
    return covers_multiple_cells;
}

bool AnalysisPoint::setCoversMultipleCells(bool multicell) const
{
    covers_multiple_cells = multicell;
    milticell_coverage_test_performed = true;
}

bool AnalysisPoint::multiCellCoverageTestPerformed() const
{
    return milticell_coverage_test_performed;
}

bool AnalysisPoint::operator==(const AnalysisPoint & other) const
{
    return (this->r == other.r &&
            this->category_id == other.category_id &&
            this->center == other.center);
}

bool AnalysisPoint::operator!=(const AnalysisPoint & other) const
{
    return !this->operator ==(other);
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
