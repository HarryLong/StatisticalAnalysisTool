#include "central_widget.h"
#include "constants.h"
#include "radial_distribution_analyzer.h"
#include "radial_distribution_reproducer.h"
#include "utils.h"
#include "point_analyzer.h"

#include <QBoxLayout>
#include <iostream>

CentralWidget::CentralWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), m_input_widget(DEFAULT_INPUT_WIDGET_DIMENSION, DEFAULT_INPUT_WIDGET_DIMENSION, this),
    m_producer_dialog(new RadialDistributionProducerDialog)
{
    connect(m_producer_dialog, SIGNAL(accepted()), this, SLOT(reproduce()));

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


    // Clear, point size & reproduce button
    {
        m_clear_btn = new QPushButton("Clear");
        m_reproduce_btn = new QPushButton("Reproduce");
        m_point_size_sb = new QSpinBox;
        m_point_size_sb->setRange(1,15);

        connect(m_clear_btn, SIGNAL(clicked()), &m_input_widget, SLOT(clear()));
        connect(m_reproduce_btn, SIGNAL(clicked()), this, SLOT(launch_reproduction_dialog()));
        connect(m_point_size_sb, SIGNAL(valueChanged(int)), &m_input_widget, SLOT(setPointSize(int)));

        QHBoxLayout * reproduce_button_layout = new QHBoxLayout;
        reproduce_button_layout->addWidget(new QLabel(""),1,Qt::AlignCenter); // Padding
        reproduce_button_layout->addWidget(m_clear_btn, 0, Qt::AlignCenter);
        reproduce_button_layout->addWidget(m_point_size_sb, 0, Qt::AlignCenter);
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

//    // Get the circles
    std::vector<AnalysisPoint*> points(m_input_widget.getPoints());

//    // Create the Analyzer
    RadialDistributionAnalyzer analyzer(reproduction_settings.r_min, reproduction_settings.r_max, reproduction_settings.r_diff);
    RadialDistribution radial_distribution(analyzer.getRadialDistribution(points, points, m_input_widget.getWidth(), m_input_widget.getHeight(), 1, 1));

//    // Write rad file
    radial_distribution.write(reproduction_settings.output_rad_file_location.toStdString());
    std::cout << "Radial distribution file written: " << reproduction_settings.output_rad_file_location.toStdString() << std::endl;

//    // Write rad csv file
    radial_distribution.writeToCSV(reproduction_settings.output_csv_file_location.toStdString());
    std::cout << "RAD CSV file written: " << reproduction_settings.output_csv_file_location.toStdString() << std::endl;

    PointSizeProperties point_size_property(PointSizeAnalyzer::getSizeProperties(points, 1, 1));
//    // Write size properties file
    point_size_property.write(reproduction_settings.output_size_properties_file_location.toStdString());
    std::cout << "Size properties file written: " << reproduction_settings.output_size_properties_file_location.toStdString() << std::endl;

//    // Write point size csv file
    point_size_property.writeToCSV(reproduction_settings.output_size_properties_csv_file_location.toStdString());
    std::cout << "Size properties CSV file written: " << reproduction_settings.output_size_properties_csv_file_location.toStdString() << std::endl;

    RadialDistributionReproducer::Settings::InitializationType init_type;
    if(reproduction_settings.init_two_points)
        init_type = RadialDistributionReproducer::Settings::InitializationType::TwoPoints;
    else if(reproduction_settings.init_match_distribution)
        init_type = RadialDistributionReproducer::Settings::InitializationType::MatchDensity;

    RadialDistributionReproducer::Settings::GenerationAlgorithm generation_algorithm;
    if(reproduction_settings.use_birth_and_death_genaration_algo)
        generation_algorithm = RadialDistributionReproducer::Settings::GenerationAlgorithm::BirthsAndDeaths;
    else if(reproduction_settings.use_random_moves_generation_algo)
        generation_algorithm = RadialDistributionReproducer::Settings::GenerationAlgorithm::RandomMoves;

//    // The magic happens
//    RadialDistributionReproducer(std::vector<RadialDistribution> radial_distributions, std::vector<PointSizeProperties> point_size_properties,
//                                 PriorityToCategoryMapper category_priorities, Settings reproduction_settings);

    std::vector<RadialDistribution> radial_distributions;
    radial_distributions.push_back(radial_distribution);

    std::vector<PointSizeProperties> point_size_properties;
    point_size_properties.push_back(point_size_property);

    RadialDistributionReproducer::PriorityToCategoryMapper category_priorities;
    category_priorities.insert(std::pair<int,int>(1,1));

    RadialDistributionReproducer::Settings settings(reproduction_settings.output_img_w, reproduction_settings.output_img_h,
                                                    reproduction_settings.n_iterations, init_type, generation_algorithm);

    RadialDistributionReproducer producer(radial_distributions, point_size_properties, category_priorities, settings);

    producer.startPointGeneration();
    std::vector<AnalysisPoint*> generated_points( producer.getGeneratedPoints() );

    FileUtils::printPointsToImg(reproduction_settings.output_img_file_location.toStdString(),
                                generated_points,
                                reproduction_settings.output_img_w,
                                reproduction_settings.output_img_h);
    std::cout << "Image file written: " << reproduction_settings.output_img_file_location.toStdString() << std::endl;

    std::string cmd("eog ");
    cmd.append(reproduction_settings.output_img_file_location.toStdString());
    cmd.append(" &");

    system(cmd.c_str());
}

void CentralWidget::launch_reproduction_dialog()
{
    m_producer_dialog->setAnalysisArea(m_input_widget.getWidth(), m_input_widget.getHeight());
    m_producer_dialog->exec();
}

void CentralWidget::setInputWidgetPoints(std::vector<AnalysisPoint*> &points)
{
    m_input_widget.setPoints(points);
}

