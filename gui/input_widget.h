#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <vector>
#include <QImage>
#include "../analysis_point.h"
#include "../utils/point_drawer.h"

#define DEFAULT_CIRCLE_RADIUS 1

/******************
 * SIGNALED LABEL *
 ******************/
class MySignaledLabel : public QLabel
{
Q_OBJECT
public:
    MySignaledLabel(int width, int height);

signals:
    void leaveEvent();
    void enterEvent();
    void mousePressed(QMouseEvent * pos);
    void mouseMoved(QMouseEvent * pos);
    void mouseReleased(QMouseEvent * pos);

protected:
    void enterEvent ( QEvent * e );

    void leaveEvent(QEvent *e);

    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);
};

class InputWidget : public QWidget{
Q_OBJECT
public:
    InputWidget(int width, int height, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~InputWidget();
    int getWidth() const;
    int getHeight() const;
    void setPoints(std::vector<AnalysisPoint> & points);
    std::map<int,std::vector<AnalysisPoint> > & getPoints();
    void addPoint(const AnalysisPoint & point);

public slots:
    void setSize(int width, int height);
    void mouse_pressed(QMouseEvent * event);
    void setPointSize(int size);
    void clear();
    void setAciveCategoryId(int category_id);

private:
    void refresh();
    void init_layout();

    int m_point_size;
    int m_active_category;
    int m_width, m_height;
    PointDrawer m_point_drawer;
    MySignaledLabel m_container_lbl;

    std::map<int,std::vector<AnalysisPoint> > m_points;
};

#endif // INPUT_WIDGET_H
