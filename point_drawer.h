#ifndef POINT_DRAWER_H
#define POINT_DRAWER_H

#include <map>
#include <vector>
#include<QImage>

class AnalysisPoint;

class PointDrawer{
public:
    PointDrawer(int width, int height);
    ~PointDrawer();

    void drawPoint(const AnalysisPoint * c);
    void reset();

    int getWidth() const;
    int getHeight() const;

    void setSize(int width, int height);
    QImage& toImage();

private:
    void init_category_to_color_map();
    QImage m_image;
    std::map<int,Qt::GlobalColor> m_category_to_color_map;
};

#endif // POINT_DRAWER_H
