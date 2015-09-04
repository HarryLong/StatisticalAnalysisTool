#include "point_drawer.h"
#include "../analysis_point.h"
#include <QPainter>

PointDrawer::PointDrawer(int width, int height) : m_image(width, height, QImage::Format_RGB32)
{
    init_category_to_color_map();
    reset();
}

PointDrawer::~PointDrawer()
{
}

void PointDrawer::drawPoint(const AnalysisPoint * p)
{
    Qt::GlobalColor color(m_category_to_color_map[p->getCategoryId()]);
    QPainter painter(&m_image);
    painter.setPen(color);
    painter.setBrush( color );
    painter.drawEllipse(p->getCenter(), p->getRadius(), p->getRadius());
    painter.end();
}

void PointDrawer::setSize(int width, int height)
{
    m_image = QImage(width,height, QImage::Format_ARGB32);
    reset();
}

void PointDrawer::reset()
{
    m_image.fill(Qt::black);
}

QImage& PointDrawer::toImage()
{
    return m_image;
}

int PointDrawer::getWidth() const
{
    return m_image.width();
}

int PointDrawer::getHeight() const
{
    return m_image.height();
}

void PointDrawer::init_category_to_color_map()
{
    m_category_to_color_map[1] = Qt::white;
    m_category_to_color_map[2] = Qt::red;
    m_category_to_color_map[3] = Qt::green;
    m_category_to_color_map[4] = Qt::blue;
    m_category_to_color_map[5] = Qt::cyan;
    m_category_to_color_map[6] = Qt::magenta;
    m_category_to_color_map[7] = Qt::yellow;
    m_category_to_color_map[8] = Qt::darkRed;
    m_category_to_color_map[9] = Qt::darkGreen;
    m_category_to_color_map[10] = Qt::darkBlue;
    m_category_to_color_map[11] = Qt::darkCyan;
    m_category_to_color_map[12] = Qt::darkMagenta;
    m_category_to_color_map[13] = Qt::darkYellow;
    m_category_to_color_map[14] = Qt::lightGray;
    m_category_to_color_map[15] = Qt::darkGray;
}
