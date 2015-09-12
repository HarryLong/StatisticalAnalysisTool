#include "input_widget.h"

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

/****************
 * INPUT WIDGET *
 ****************/
InputWidget::InputWidget(int width, int height, QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), m_width(width), m_height(height),
    m_point_drawer(width, height), m_container_lbl(width, height),
    m_point_size(1), m_active_category(1), m_points()
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

void InputWidget::addPoint(const AnalysisPoint & point)
{
    if(m_points.find(m_active_category) == m_points.end())
        m_points.insert(std::pair<int, std::vector<AnalysisPoint> >(m_active_category, std::vector<AnalysisPoint>()));
    m_points[m_active_category].push_back(point);
    m_point_drawer.drawPoint(point);
    refresh();
}

void InputWidget::setSize(int width, int height)
{
    m_container_lbl.setFixedSize(width, height);
    m_point_drawer.setSize(width, height);
    // Iterate through each point and reset their center coordinate
    for(auto categories_it(m_points.begin()); categories_it != m_points.end(); categories_it++)
    {
        for(auto points_it(categories_it->second.begin()); points_it != categories_it->second.end(); points_it++)
        {
            AnalysisPoint & point (*points_it);
            QPoint new_center(
                        (((float)point.getCenter().x())/m_width) * width, //x
                        (((float)point.getCenter().y())/m_height) * height
                        );
            point.setCenter(new_center);

            m_point_drawer.drawPoint(point);
        }
    }
    m_width = width;
    m_height = height;
    refresh();
}

void InputWidget::refresh()
{
    m_container_lbl.setPixmap(QPixmap::fromImage(m_point_drawer.toImage()));
}

void InputWidget::mouse_pressed(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
        addPoint(AnalysisPoint(m_active_category, event->pos(), m_point_size));
}

void InputWidget::clear()
{
    m_points.clear();
    m_point_drawer.reset();
    refresh();
}

void InputWidget::setPoints(std::vector<AnalysisPoint> &points)
{
    m_points.clear();
    m_point_drawer.reset();
    for(AnalysisPoint & p : points)
        addPoint(p);

    refresh();
}

int InputWidget::getWidth() const
{
    return m_width;
}

int InputWidget::getHeight() const
{
    return m_height;
}

std::map<int,std::vector<AnalysisPoint> > & InputWidget::getPoints()
{
    return m_points;
}

void InputWidget::setPointSize(int size)
{
    m_point_size = size;
}

void InputWidget::setAciveCategoryId(int category_id)
{
    m_active_category = category_id;
}
