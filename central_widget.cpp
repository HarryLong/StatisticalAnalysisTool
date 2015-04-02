#include "central_widget.h"
#include "constants.h"
#include "radial_distribution_analyzer.h"
#include "radial_distribution_producer.h"
#include "utils.h"

#include <QBoxLayout>
#include <iostream>

CentralWidget::CentralWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), m_input_widget(DEFAULT_INPUT_WIDGET_DIMENSION, DEFAULT_INPUT_WIDGET_DIMENSION, this),
    m_producer_dialog(new RadialDistributionProducerDialog)
{    
    init_layout();
}

CentralWidget::~CentralWidget()
{
    delete m_producer_dialog;
}

void CentralWidget::setInputWidgetSize(int width, int height)
{
    m_input_widget.setSize(width, height);
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

    // Reproduce button
    {
        m_clear_btn = new QPushButton("Clear");
        m_reproduce_btn = new QPushButton("Reproduce");
        connect(m_clear_btn, SIGNAL(clicked()), &m_input_widget, SLOT(clear()));
        connect(m_reproduce_btn, SIGNAL(clicked()), this, SLOT(launch_reproduction_dialog()));

        QHBoxLayout * reproduce_button_layout = new QHBoxLayout;
        reproduce_button_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding
        reproduce_button_layout->addWidget(m_clear_btn, 0, Qt::AlignCenter);
        reproduce_button_layout->addWidget(m_reproduce_btn, 0, Qt::AlignCenter);
        reproduce_button_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding
        layout->addLayout(reproduce_button_layout,0);
    }

    setLayout(layout);
}

void CentralWidget::reproduce()
{
    // Get the reproduction settings
    ReproductionSettings reproduction_settings(m_producer_dialog->getReproductionSettings());

    // Get the circles
    std::vector<QPoint> points;
    for(Circle c : m_input_widget.getCircles())
        points.push_back(c.center);

    // Create the Analyzer
    RadialDistributionAnalyzer analyzer(reproduction_settings.r_min, reproduction_settings.r_max, reproduction_settings.r_diff);
    RadialDistribution radial_distribution(analyzer.getRadialDistribution(points, points, m_input_widget.getWidth(), m_input_widget.getHeight(), 0, 1));

    // Write rad file
    radial_distribution.write(reproduction_settings.output_rad_file_location.toStdString());
    std::cout << "Radial distribution file written: " << reproduction_settings.output_rad_file_location.toStdString() << std::endl;

    // Write csv file
    radial_distribution.writeToCSV(reproduction_settings.output_csv_file_location.toStdString());
    std::cout << "CSV file written: " << reproduction_settings.output_csv_file_location.toStdString() << std::endl;

    RadialDistributionProducer producer(radial_distribution);
    FileUtils::printPointsToImg(reproduction_settings.output_img_file_location.toStdString(),
                                producer.generatePoints(reproduction_settings.output_img_w, reproduction_settings.output_img_h, reproduction_settings.n_iterations, true),
                                reproduction_settings.output_img_w,
                                reproduction_settings.output_img_h);
    std::cout << "Image file written: " << reproduction_settings.output_img_file_location.toStdString() << std::endl;

    std::string cmd("eog ");
    cmd.append(reproduction_settings.output_img_file_location.toStdString());
    system(cmd.c_str());
}

void CentralWidget::launch_reproduction_dialog()
{
    m_producer_dialog->setAnalysisArea(m_input_widget.getWidth(), m_input_widget.getHeight());
    connect(m_producer_dialog, SIGNAL(accepted()), this, SLOT(reproduce()));
    m_producer_dialog->exec();
}

void CentralWidget::setInputWidgetPoints(std::vector<QPoint> &points)
{
    m_input_widget.setPoints(points);
}

