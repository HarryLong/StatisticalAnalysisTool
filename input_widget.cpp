#include "input_widget.h"
#include "constants.h"

#include <QPainter>
#include <QBoxLayout>
#include <QMouseEvent>
#include <iostream>

/******************
 * SIGNALED LABEL *
 ******************/
MySignaledLabel::MySignaledLabel(int width, int height)
{
    setFixedSize(width, height);
}

void MySignaledLabel::enterEvent ( QEvent * e ) {
    QLabel::enterEvent(e);
    emit enterEvent();
}

void MySignaledLabel::leaveEvent(QEvent *e){
    QLabel::leaveEvent(e);
    emit leaveEvent();
}

void MySignaledLabel::mousePressEvent(QMouseEvent *e)
{
    QLabel::mousePressEvent(e);
    emit mousePressed(e);
}

void MySignaledLabel::mouseMoveEvent(QMouseEvent *e)
{
    QLabel::mouseMoveEvent(e);
    emit mouseMoved(e);
}

void MySignaledLabel::mouseReleaseEvent(QMouseEvent *e)
{
    QLabel::mouseReleaseEvent(e);
    emit mouseReleased(e);
}

/**************
 * PIXEL DATA *
 **************/
PixelData::PixelData(int width, int height) : m_image(width, height, QImage::Format_ARGB32)
{
    reset();
}

PixelData::~PixelData()
{
}

void PixelData::addCircle(Circle circle)
{
    m_circles.push_back(circle);
    drawCircle(circle);
}

void PixelData::addCircles(std::vector<Circle> circles)
{
    for(Circle c : circles)
        addCircle(c);
}

void PixelData::drawCircle(const Circle& c)
{
    QPainter painter(&m_image);
    painter.setPen(Qt::white);
    painter.setBrush( Qt::white );
    painter.drawEllipse(c.center, c.radius, c.radius);
    painter.end();
}

void PixelData::setSize(int width, int height)
{
    for(Circle & c : m_circles)
    {
        c.center.setX((((float)c.center.x())/m_image.width()) * width);
        c.center.setY((((float)c.center.y())/m_image.height()) * height);
    }

    m_image = QImage(width,height, QImage::Format_ARGB32);
    m_image.fill(Qt::black);

    for(Circle c : m_circles)
        drawCircle(c);
}

void PixelData::reset()
{
    m_circles.clear();
    m_image.fill(Qt::black);
}

QImage& PixelData::toImage()
{
    return m_image;
}

int PixelData::getWidth() const
{
    return m_image.width();
}

int PixelData::getHeight() const
{
    return m_image.height();
}

std::vector<Circle> & PixelData::getCircles()
{
    return m_circles;
}

/****************
 * INPUT WIDGET *
 ****************/
InputWidget::InputWidget(int width, int height, QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), m_pixel_data(width, height), m_container_lbl(width, height)
{
    connect(&m_container_lbl, SIGNAL(mousePressed(QMouseEvent*)), this, SLOT(mouse_pressed(QMouseEvent*)));

    init_layout();
    refresh();
}

InputWidget::~InputWidget()
{

}

void InputWidget::init_layout()
{
    QHBoxLayout * layout = new QHBoxLayout;

    layout->addWidget(&m_container_lbl, 0, Qt::AlignCenter);

    setLayout(layout);
}

void InputWidget::setSize(int width, int height)
{
    m_container_lbl.setFixedSize(width, height);
    m_pixel_data.setSize(width, height);
    refresh();
}

void InputWidget::refresh()
{
    m_container_lbl.setPixmap(QPixmap::fromImage(m_pixel_data.toImage()));
}

void InputWidget::mouse_pressed(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        m_pixel_data.addCircle(event->pos());
        refresh();
    }
}

void InputWidget::clear()
{
    m_pixel_data.reset();
    refresh();
}

void InputWidget::setPoints(std::vector<QPoint> &points)
{
    m_pixel_data.reset();
    for(QPoint p : points)
        m_pixel_data.addCircle(Circle(p));

    refresh();
}

int InputWidget::getWidth() const
{
    return m_pixel_data.getWidth();
}

int InputWidget::getHeight() const
{
    return m_pixel_data.getHeight();
}

std::vector<Circle> & InputWidget::getCircles()
{
    return m_pixel_data.getCircles();
}
