#include "main_window.h"
#include <QBoxLayout>
#include "constants.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), m_central_widget(new CentralWidget(this)),
    m_random_distribution_producer_dlg(new RandomDistributionProducerDialog),
    m_clustered_distribution_producer_dlg(new ClusteredDistributionProducerDialog),
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

    // Dialog connections
    connect(m_random_distribution_producer_dlg, SIGNAL(accepted()), this, SLOT(generate_random_distribution()));
    connect(m_clustered_distribution_producer_dlg, SIGNAL(accepted()), this, SLOT(generate_clustered_distribution()));
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
    std::vector<QPoint> generated_points( m_distribution_factory.generateRandomDistribution(n_points, m_input_widget_width, m_input_widget_height) );
    m_central_widget->setInputWidgetPoints( generated_points );
}

void MainWindow::generate_clustered_distribution()
{
    int n_seed_points( m_clustered_distribution_producer_dlg->getNSeedPoints() );
    int n_seeding_iterations( m_clustered_distribution_producer_dlg->getNSeedingIterations() );
    int max_seeding_distance( m_clustered_distribution_producer_dlg->getMaxSeedingDistance() );

    std::vector<QPoint> generated_points( m_distribution_factory.generateSeededDistribution(n_seed_points, n_seeding_iterations, max_seeding_distance,
                                                                                            m_input_widget_width, m_input_widget_height)  );

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
