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
    int getActiveCategoryId();

private slots:
    void launch_analysis_configuration_producer_dialog();
    void analyse();

private:
    void init_layout();

    InputWidget m_input_widget;
    QSpinBox * m_point_size_sb;
    QSpinBox * m_category_sb;

    QPushButton * m_analyze_btn;

    QPushButton * m_clear_btn;

    AnalysisConfigurationProducerDialog * m_analysis_configuration_producer_dialog;
};

#endif // CENTRAL_WIDGET_H
