#include "central_widget.h"
#include "constants.h"
#include "radial_distribution_analyzer.h"
#include "utils.h"
#include "category_analyzer.h"
#include "analyzer.h"

#include <QBoxLayout>
#include <iostream>

CentralWidget::CentralWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), m_input_widget(DEFAULT_INPUT_WIDGET_DIMENSION, DEFAULT_INPUT_WIDGET_DIMENSION, this),
    m_analysis_configuration_producer_dialog(new AnalysisConfigurationProducerDialog)
{
    connect(m_analysis_configuration_producer_dialog, SIGNAL(accepted()), this, SLOT(analyse()));
    setInputWidgetSize(DEFAULT_INPUT_WIDGET_DIMENSION, DEFAULT_INPUT_WIDGET_DIMENSION);

    init_layout();
}

CentralWidget::~CentralWidget()
{
    delete m_analysis_configuration_producer_dialog;
}

void CentralWidget::setInputWidgetSize(int width, int height)
{
    m_analysis_configuration_producer_dialog->setAnalysisWindowDimensions(width, height);
    m_input_widget.setSize(width, height);
}

void CentralWidget::launch_analysis_configuration_producer_dialog()
{
    m_analysis_configuration_producer_dialog->exec();
}

void CentralWidget::setInputWidgetPoints(std::vector<AnalysisPoint*> &points)
{
    m_input_widget.setPoints(points);
}

int CentralWidget::getActiveCategoryId()
{
    return m_category_sb->value();
}

void CentralWidget::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    // The input
    {
        QHBoxLayout * input_widget_layout = new QHBoxLayout;
        input_widget_layout->addWidget(&m_input_widget, 0, Qt::AlignCenter);
        layout->addLayout(input_widget_layout,1);
    }

    // Point size & category
    {
        m_point_size_sb = new QSpinBox;
        m_point_size_sb->setRange(1,15);
        m_category_sb = new QSpinBox;
        m_category_sb->setRange(1,15);
        m_category_sb->setValue(1);

        connect(m_point_size_sb, SIGNAL(valueChanged(int)), &m_input_widget, SLOT(setPointSize(int)));
        connect(m_category_sb, SIGNAL(valueChanged(int)), &m_input_widget, SLOT(setAciveCategoryId(int)));

        QHBoxLayout * ps_and_category_id_layout = new QHBoxLayout;
        ps_and_category_id_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding

        ps_and_category_id_layout->addWidget(new QLabel("Point size: "), 0, Qt::AlignCenter);
        ps_and_category_id_layout->addWidget(m_point_size_sb, 0, Qt::AlignCenter);
        ps_and_category_id_layout->addWidget(new QLabel("Category: "), 0, Qt::AlignCenter);
        ps_and_category_id_layout->addWidget(m_category_sb, 0, Qt::AlignCenter);

        ps_and_category_id_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding
        layout->addLayout(ps_and_category_id_layout,0);
    }

    // Clear, point size , category & reproduce button
    {
        m_clear_btn = new QPushButton("Clear");
        m_analyze_btn = new QPushButton("Analyse");

        connect(m_clear_btn, SIGNAL(clicked()), &m_input_widget, SLOT(clear()));
        connect(m_analyze_btn, SIGNAL(clicked()), this, SLOT(launch_analysis_configuration_producer_dialog()));

        QHBoxLayout * clear_and_reproduce_button_layout = new QHBoxLayout;
        clear_and_reproduce_button_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding

        clear_and_reproduce_button_layout->addWidget(m_clear_btn, 0, Qt::AlignCenter);
        clear_and_reproduce_button_layout->addWidget(m_analyze_btn, 0, Qt::AlignCenter);

        clear_and_reproduce_button_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding
        layout->addLayout(clear_and_reproduce_button_layout,0);
    }

    setLayout(layout);
}

void CentralWidget::analyse()
{
    // Get analysis configuration
    AnalysisConfiguration analysis_configuration( m_analysis_configuration_producer_dialog->getConfiguration() );

    // Get analysis points
    std::map<int,std::vector<AnalysisPoint*> > categorised_points(m_input_widget.getPoints());

    // Build vector of categories sorted by priority. Here, we simply use the category id as priority (1 higher than 2)
    for(auto it(categorised_points.begin()); it != categorised_points.end(); it++)
        analysis_configuration.priority_sorted_category_ids.push_back(it->first);

    Analyzer::generate_statistical_data(m_analysis_configuration_producer_dialog->getOutputDir(), categorised_points,
                                        analysis_configuration, true);
}

