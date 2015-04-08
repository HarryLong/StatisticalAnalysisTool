#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include "input_widget.h"
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include "dialogs.h"

class AnalysisPoint;
class CentralWidget : public QWidget{
Q_OBJECT
public:
    CentralWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CentralWidget();
    void setInputWidgetSize(int width, int height);
    void setInputWidgetPoints(std::vector<AnalysisPoint*>& points);

private slots:
    void launch_reproduction_dialog();
    void reproduce();

private:
    void init_layout();

    InputWidget m_input_widget;
    QSpinBox * m_point_size_sb;
    QPushButton * m_reproduce_btn;
    QPushButton * m_clear_btn;

    RadialDistributionProducerDialog * m_producer_dialog;
};

#endif // CENTRAL_WIDGET_H
