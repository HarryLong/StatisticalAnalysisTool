#include "main_window.h"
#include <QBoxLayout>
#include <QMenuBar>
#include "../analysis_point.h"
#include "../reproducer/reproducer.h"
#include "../utils/utils.h"
#include "central_widget.h"
#include "dialogs.h"
#include "reproduction_config_dialog.h"
#include <QActionGroup>

#define MAX_INPUT_WIDGET_DIMENSION 1000
#define MIN_INPUT_WIDGET_DIMENSION 100
#define INPUT_WIDGET_DIMENSION_INCREMENTS 100
#define DEFAULT_INPUT_WIDGET_DIMENSION 500

#include <thread>

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent, flags),
    m_central_widget(new CentralWidget(DEFAULT_INPUT_WIDGET_DIMENSION, this)),
    m_random_distribution_producer_dlg(new RandomDistributionProducerDialog),
    m_clustered_distribution_producer_dlg(new ClusteredDistributionProducerDialog),
    m_reproduction_configuration_producer_dlg(new ReproductionConfigurationProducerDialog),
    m_input_widget_width(DEFAULT_INPUT_WIDGET_DIMENSION),
    m_input_widget_height(DEFAULT_INPUT_WIDGET_DIMENSION)
{
    init_actions();
    init_menu();
    init_layout();
}

MainWindow::~MainWindow()
{
    for(QAction * a : m_size_ag->actions())
        delete a;
    delete m_size_ag;
    delete m_size_menu;
    delete m_distributions_menu;
    delete m_generate_random_distribution_action;
    delete m_generate_clustered_action;
    delete m_reproduction_configuration_producer_dlg;
    delete m_clustered_distribution_producer_dlg;
    delete m_random_distribution_producer_dlg;
}

void MainWindow::closeEvent ( QCloseEvent * event )
{

}

void MainWindow::init_layout()
{
    setCentralWidget(m_central_widget);
}


void MainWindow::init_actions()
{
    // Size Actions
    m_size_ag = new QActionGroup( this );

    for(int size(MIN_INPUT_WIDGET_DIMENSION); size <= MAX_INPUT_WIDGET_DIMENSION; size += INPUT_WIDGET_DIMENSION_INCREMENTS)\
    {
        QAction * action = new QAction(QString::number(size), this);
        action->setCheckable(true);
        action->setActionGroup(m_size_ag);
        if(size == DEFAULT_INPUT_WIDGET_DIMENSION)
            action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(refresh_input_widget_size()));
    }

    // Distribution Actions
    m_generate_random_distribution_action = new QAction("Random", this);
    connect(m_generate_random_distribution_action, SIGNAL(triggered()), this, SLOT(show_random_distribution_dialog()));

    m_generate_clustered_action = new QAction("Clustered", this);
    connect(m_generate_clustered_action, SIGNAL(triggered()), this, SLOT(show_clustered_distribution_dialog()));

    // Reproduction actions
    m_launch_reproduction_dialog_action = new QAction("Reproduce", this);
    connect(m_launch_reproduction_dialog_action, SIGNAL(triggered()), this, SLOT(launch_reproduction_configuration_producer_dialog()));

    // Dialog connections
    connect(m_random_distribution_producer_dlg, SIGNAL(accepted()), this, SLOT(generate_random_distribution()));
    connect(m_clustered_distribution_producer_dlg, SIGNAL(accepted()), this, SLOT(generate_clustered_distribution()));
    connect(m_reproduction_configuration_producer_dlg, SIGNAL(accepted()), this, SLOT(reproduce()));
}

void MainWindow::init_menu()
{
    // Size
    m_size_menu = menuBar()->addMenu("Size");
    for(QAction * a : m_size_ag->actions())
        m_size_menu->addAction(a);

    // Distributions
    m_distributions_menu = menuBar()->addMenu("Distribution Factory");
    m_distributions_menu->addAction(m_generate_random_distribution_action);
    m_distributions_menu->addAction(m_generate_clustered_action);

    // Reproduce
    m_reproduction_menu = menuBar()->addMenu("Reproduce");
    m_reproduction_menu->addAction(m_launch_reproduction_dialog_action);
}

void MainWindow::refresh_input_widget_size()
{
    QAction * selected_action(m_size_ag->checkedAction());

    int size(selected_action->text().toInt());

    m_input_widget_width = m_input_widget_height = size;

    m_central_widget->setInputWidgetSize(size,size);
}

void MainWindow::generate_random_distribution()
{
    int n_points(m_random_distribution_producer_dlg->getNPoints());
    int min_point_radius( m_random_distribution_producer_dlg->getMinimumRadius() );
    int max_point_radius( m_random_distribution_producer_dlg->getMaximumRadius() );

    std::vector<AnalysisPoint> generated_points( m_distribution_factory.generateRandomDistribution(m_central_widget->getActiveCategoryId(), n_points, m_input_widget_width, m_input_widget_height,
                                                                                                    min_point_radius, max_point_radius) );
    m_central_widget->setInputWidgetPoints( generated_points );
}

void MainWindow::generate_clustered_distribution()
{
    int n_seed_points( m_clustered_distribution_producer_dlg->getNSeedPoints() );
    int n_seeding_iterations( m_clustered_distribution_producer_dlg->getNSeedingIterations() );
    int max_seeding_distance( m_clustered_distribution_producer_dlg->getMaxSeedingDistance() );
    bool equidistant_seeds( m_clustered_distribution_producer_dlg->equidistantSeeding() );
    int min_point_radius( m_clustered_distribution_producer_dlg->getMinimumRadius() );
    int max_point_radius( m_clustered_distribution_producer_dlg->getMaximumRadius() );

    std::vector<AnalysisPoint> generated_points( m_distribution_factory.generateSeededDistribution(m_central_widget->getActiveCategoryId(), n_seed_points, n_seeding_iterations, max_seeding_distance,
                                                                                            m_input_widget_width, m_input_widget_height,
                                                                                            min_point_radius, max_point_radius, equidistant_seeds)  );

    m_central_widget->setInputWidgetPoints( generated_points );
}

void MainWindow::show_random_distribution_dialog()
{
    m_random_distribution_producer_dlg->exec();
}

void MainWindow::show_clustered_distribution_dialog()
{
    m_clustered_distribution_producer_dlg->exec();
}

void MainWindow::launch_reproduction_configuration_producer_dialog()
{
    m_reproduction_configuration_producer_dlg->exec();
}

#include "../analyser/analyzer.h"
#include "../analyser/analysis_configuration.h"

void MainWindow::reproduce()
{
    ReproductionConfiguration reproduction_config( m_reproduction_configuration_producer_dlg->getConfiguration() );

//    RadialDistributionReproducer::reproduce(reproduction_config);

    std::map<int,std::vector<AnalysisPoint> > reproduced_points (RadialDistributionReproducer::reproduce(reproduction_config));
    ImageUtils::printPointsToImg(m_reproduction_configuration_producer_dlg->getOutputFilename().toStdString(),
                                     reproduced_points, reproduction_config.width, reproduction_config.height);

//    AnalysisConfiguration config(0, 200, 20, 10000, 10000);
//    std::vector<int> priority_sorted_cat;
//    priority_sorted_cat.push_back(9);
//    priority_sorted_cat.push_back(5);
//    priority_sorted_cat.push_back(6);

//    config.setPrioritySortedCategoryIds(priority_sorted_cat);

//    // Reanalyze
//    Analyzer::analyze("/home/harry/.ecotracker/reanalyzed/", reproduced_points, config);

    // Open the file
//    std::string cmd("eog ");
//    cmd.append(m_reproduction_configuration_producer_dlg->getOutputFile().toStdString());
//    cmd.append(" &");
//    system(cmd.c_str());

//    std::vector<std::thread*> threads;

//    for(int i(0); i < 100; i++)
//        threads.push_back(new std::thread(RadialDistributionReproducer::reproduce,reproduction_config));

//    for(std::thread * t : threads)
//    {
//        t->join();
//        delete t;
//    }

    //SINGLE


////    // Open the file
//    std::string cmd("eog ");
//    cmd.append(m_reproduction_configuration_producer_dlg->getOutputFile().toStdString());
//    cmd.append(" &");
//    system(cmd.c_str());
}
