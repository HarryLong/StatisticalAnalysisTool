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
    for(AnalysisPoint * p : m_points)
        delete p;
}

void PixelData::addPoint(AnalysisPoint * point)
{
    m_points.push_back(point);
    drawPoint(point);
}

void PixelData::addPoints(std::vector<AnalysisPoint *> points)
{
    for(AnalysisPoint * p : points)
        addPoint(p);
}

void PixelData::drawPoint(const AnalysisPoint * p)
{
    QPainter painter(&m_image);
    painter.setPen(Qt::white);
    painter.setBrush( Qt::white );
    painter.drawEllipse(p->getCenter(), p->getRadius(), p->getRadius());
    painter.end();
}

void PixelData::setSize(int width, int height)
{
    for(AnalysisPoint * p : m_points)
    {
        QPoint new_center(
                    (((float)p->getCenter().x())/m_image.width()) * width, //x
                    (((float)p->getCenter().y())/m_image.height()) * height
                    );
        p->setCenter(new_center);
    }

    m_image = QImage(width,height, QImage::Format_ARGB32);
    m_image.fill(Qt::black);

    for(AnalysisPoint * p : m_points)
        drawPoint(p);
}

void PixelData::reset()
{
    for(AnalysisPoint* p : m_points)
        delete p;
    m_points.clear();
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

std::vector<AnalysisPoint*> & PixelData::getPoints()
{
    return m_points;
}

/****************
 * INPUT WIDGET *
 ****************/
InputWidget::InputWidget(int width, int height, QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), m_pixel_data(width, height), m_container_lbl(width, height),
    m_point_size(1), m_active_category(1)
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
        m_pixel_data.addPoint(new AnalysisPoint(m_active_category, event->pos(), m_point_size));
        refresh();
    }
}

void InputWidget::clear()
{
    m_pixel_data.reset();
    refresh();
}

void InputWidget::setPoints(std::vector<AnalysisPoint*> &points)
{
    m_pixel_data.reset();
    for(AnalysisPoint* p : points)
        m_pixel_data.addPoint(p);

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

std::vector<AnalysisPoint*> & InputWidget::getPoints()
{
    return m_pixel_data.getPoints();
}

void InputWidget::setPointSize(int size)
{
    m_point_size = size;
}
