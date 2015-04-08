#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <vector>
#include <QImage>
#include "analysis_point.h"

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

class PixelData{
public:
    PixelData(int width, int height);
    ~PixelData();

    void addPoint(AnalysisPoint * center);
    void addPoints(std::vector<AnalysisPoint*> points);
    void drawPoint(const AnalysisPoint * c);
    void reset();

    int getWidth() const;
    int getHeight() const;

    void setSize(int width, int height);
    QImage& toImage();
    std::vector<AnalysisPoint*> & getPoints();

private:
    std::vector<AnalysisPoint*> m_points;
    QImage m_image;
};

class InputWidget : public QWidget{
Q_OBJECT
public:
    InputWidget(int width, int height, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~InputWidget();
    int getWidth() const;
    int getHeight() const;
    void setPoints(std::vector<AnalysisPoint*> & points);
    std::vector<AnalysisPoint*> & getPoints();

public slots:
    void setSize(int width, int height);
    void mouse_pressed(QMouseEvent * event);
    void setPointSize(int size);
    void clear();

private:
    void refresh();
    void init_layout();

    int m_point_size;
    int m_active_category;
    PixelData m_pixel_data;
    MySignaledLabel m_container_lbl;
};

#endif // INPUT_WIDGET_H
