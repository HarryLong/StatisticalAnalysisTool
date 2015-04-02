#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <vector>
#include <QImage>

#define DEFAULT_CIRCLE_RADIUS 1
struct Circle
{
public:
    Circle(QPoint center, int radius = DEFAULT_CIRCLE_RADIUS) : center(center), radius(radius) {}
    QPoint center;
    int radius;
};

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

    void addCircle(Circle center);
    void addCircles(std::vector<Circle> points);
    void drawCircle(const Circle & c);
    void reset();

    int getWidth() const;
    int getHeight() const;

    void setSize(int width, int height);
    QImage& toImage();
    std::vector<Circle> & getCircles();

private:
    std::vector<Circle> m_circles;
    QImage m_image;
};

class InputWidget : public QWidget{
Q_OBJECT
public:
    InputWidget(int width, int height, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~InputWidget();
    int getWidth() const;
    int getHeight() const;
    void setPoints(std::vector<QPoint> & points);
    std::vector<Circle> & getCircles();

public slots:
    void setSize(int width, int height);
    void mouse_pressed(QMouseEvent * event);
    void clear();

private:
    void refresh();
    void init_layout();

    PixelData m_pixel_data;
    MySignaledLabel m_container_lbl;
};

#endif // INPUT_WIDGET_H
