#include "../gui/dialogs.h"
#include <algorithm>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>

/******************************************
 * ANALYSIS CONFIGURATION PRODUCER DIALOG *
 ******************************************/
#define R_MIN_RANGE_MIN 0
#define R_MIN_RANGE_MAX 100
#define R_MIN_DEFAULT 0
#define R_MAX_RANGE_MIN 10
#define R_DIFF_RANGE_MIN 1
#define R_DIFF_RANGE_MAX 200
#define R_DIFF_DEFAULT 5
AnalysisConfigurationProducerDialog::AnalysisConfigurationProducerDialog()
{
    setModal(true);

    // R
    m_r_min_sb = new QSpinBox;
    m_r_min_sb->setRange(R_MIN_RANGE_MIN, R_MIN_RANGE_MAX);
    m_r_min_sb->setValue(R_MIN_DEFAULT);

    m_r_max_sb = new QSpinBox;

    m_r_diff_sb = new QSpinBox;
    m_r_diff_sb->setRange(R_DIFF_RANGE_MIN, R_DIFF_RANGE_MAX);
    m_r_diff_sb->setValue(R_DIFF_DEFAULT);

    // Output dir
    m_output_dir_te = new QLineEdit;
    m_output_dir_te->setEnabled(false);
//    m_output_dir_te->setText(DEFAULT_ACTIVE_DIR);

    // Change output dir button
    m_change_output_dir_btn = new QPushButton("...", this);
    connect(m_change_output_dir_btn, SIGNAL(clicked()), this, SLOT(change_output_directory()));

    // OK button
    m_ok_btn = new QPushButton("Start");
    m_ok_btn->setEnabled(false);
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));

    init_layout();
}

AnalysisConfigurationProducerDialog::~AnalysisConfigurationProducerDialog()
{

}

void AnalysisConfigurationProducerDialog::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    // R settings
    {
        QHBoxLayout * r_layout = new QHBoxLayout;
        // Rmin
        r_layout->addWidget(new QLabel("R min: "),0);
        r_layout->addWidget(m_r_min_sb,0);
        // Rmax
        r_layout->addWidget(new QLabel("   |   R max: "),0);
        r_layout->addWidget(m_r_max_sb, 0);
        // Rdiff
        r_layout->addWidget(new QLabel("   |   R diff: "),0);
        r_layout->addWidget(m_r_diff_sb, 0);
        // Padding
        r_layout->addWidget(new QLabel(""),1);

        layout->addLayout(r_layout,0);
    }

    // Output dir
    {
        QHBoxLayout * output_dir_layout = new QHBoxLayout;
        output_dir_layout->addWidget(new QLabel("Output directory: "),0);
        output_dir_layout->addWidget(m_output_dir_te,1);
        output_dir_layout->addWidget(m_change_output_dir_btn,0);
        layout->addLayout(output_dir_layout,0);
    }

    // Padding
    QHBoxLayout * padding_layout = new QHBoxLayout;
    padding_layout->addWidget(new QLabel(""),1, Qt::AlignCenter);
    layout->addLayout(padding_layout,1);

    // OK button
    {
        QHBoxLayout * padding_layout = new QHBoxLayout;
        padding_layout->addWidget(m_ok_btn,0, Qt::AlignCenter);
        layout->addLayout(padding_layout,0);
    }

    setLayout(layout);
}

void AnalysisConfigurationProducerDialog::change_output_directory()
{
    QString output_dir = QFileDialog::getExistingDirectory(this,
        tr("Set output folder"), m_output_dir_te->text());

    if(output_dir != QString::null)
    {
        m_output_dir_te->setText(output_dir);
        m_ok_btn->setEnabled(true);
    }
}

QString AnalysisConfigurationProducerDialog::getOutputDir()
{
    return m_output_dir_te->text();
}

void AnalysisConfigurationProducerDialog::setAnalysisWindowDimensions(int width, int height)
{
    m_analysis_window_width = width;
    m_analysis_window_height = height;
    m_r_max_sb->setRange(R_MAX_RANGE_MIN, std::max(width, height));
    m_r_max_sb->setValue(std::max(width, width)/4.0f);
}

AnalysisConfiguration AnalysisConfigurationProducerDialog::getConfiguration()
{
    return AnalysisConfiguration(m_r_min_sb->value(), m_r_max_sb->value(), m_r_diff_sb->value(), m_analysis_window_width, m_analysis_window_height);
}

/****************************************
 * RANDOM DISTSRIBUTION PRODUCER DIALOG *
 ****************************************/
#define DEFAULT_N_POINTS 100
RandomDistributionProducerDialog::RandomDistributionProducerDialog()
{
    setModal(true);

    m_n_points = new QLineEdit;
    m_n_points->setText(QString::number(DEFAULT_N_POINTS));

    m_size_min_le = new QLineEdit;
    m_size_min_le->setText(QString::number(1));
    m_size_max_le = new QLineEdit;
    m_size_max_le->setText(QString::number(1));

    m_ok_btn = new QPushButton("OK");
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));

    init_layout();
}

RandomDistributionProducerDialog::~RandomDistributionProducerDialog()
{

}

void RandomDistributionProducerDialog::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    // R settings
    {
        QHBoxLayout * n_points_layout = new QHBoxLayout;
        // Rmin
        n_points_layout->addWidget(new QLabel("Number of points: "),0);
        n_points_layout->addWidget(m_n_points,0);
        // Padding
        n_points_layout->addWidget(new QLabel(""),0);

        layout->addLayout(n_points_layout,1);
    }

    // min max
    {
        QHBoxLayout * point_size_layout = new QHBoxLayout;
        // Rmin
        point_size_layout->addWidget(new QLabel("Point sizes: from "),0);
        point_size_layout->addWidget(m_size_min_le,0);
        point_size_layout->addWidget(new QLabel(" to "),0);
        point_size_layout->addWidget(m_size_max_le,0);
        // Padding
        point_size_layout->addWidget(new QLabel(""),1);

        layout->addLayout(point_size_layout,0);
    }

    // Padding
    QHBoxLayout * padding_layout = new QHBoxLayout;
    padding_layout->addWidget(new QLabel(""),1, Qt::AlignCenter);
    layout->addLayout(padding_layout,1);

    // Start button
    {
        QHBoxLayout * ok_btn_layout = new QHBoxLayout;
        ok_btn_layout->addWidget(m_ok_btn,0, Qt::AlignCenter);
        layout->addLayout(ok_btn_layout,0);
    }

    setLayout(layout);
}

int RandomDistributionProducerDialog::getNPoints()
{
    return m_n_points->text().toInt();
}

int RandomDistributionProducerDialog::getMinimumRadius()
{
    return m_size_min_le->text().toInt();
}

int RandomDistributionProducerDialog::getMaximumRadius()
{
    return m_size_max_le->text().toInt();
}

/*******************************************
 * CLUSTERED DISTSRIBUTION PRODUCER DIALOG *
 *******************************************/
#define DEFAULT_N_SEEDING_POINTS 10
#define DEFAULT_N_SEEDING_ITERATIONS 100
#define DEFAULT_MAX_SEEDING_DISTANCE 20
ClusteredDistributionProducerDialog::ClusteredDistributionProducerDialog()
{
    setModal(true);

    m_n_seed_points = new QLineEdit;
    m_n_seed_points->setText(QString::number(DEFAULT_N_SEEDING_POINTS));

    m_n_seeding_iterations = new QLineEdit;
    m_n_seeding_iterations->setText(QString::number(DEFAULT_N_SEEDING_ITERATIONS));

    m_max_seeding_distance = new QLineEdit;
    m_max_seeding_distance->setText(QString::number(DEFAULT_MAX_SEEDING_DISTANCE));

    m_equidistant_cb = new QCheckBox;
    m_equidistant_cb->setChecked(false);

    m_size_min_le = new QLineEdit;
    m_size_min_le->setText(QString::number(1));
    m_size_max_le = new QLineEdit;
    m_size_max_le->setText(QString::number(1));

    m_ok_btn = new QPushButton("OK");
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));

    init_layout();
}

ClusteredDistributionProducerDialog::~ClusteredDistributionProducerDialog()
{

}

void ClusteredDistributionProducerDialog::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    {
        QHBoxLayout * n_seeding_points_layout = new QHBoxLayout;
        // Rmin
        n_seeding_points_layout->addWidget(new QLabel("Number of seeding points: "),0);
        n_seeding_points_layout->addWidget(m_n_seed_points,0);
        // Padding
        n_seeding_points_layout->addWidget(new QLabel(""),1);

        layout->addLayout(n_seeding_points_layout,0);
    }

    {
        QHBoxLayout * n_max_distance_layout = new QHBoxLayout;
        // Rmin
        n_max_distance_layout->addWidget(new QLabel("Max seeding distance: "),0);
        n_max_distance_layout->addWidget(m_max_seeding_distance,0);
        // Padding
        n_max_distance_layout->addWidget(new QLabel(""),1);

        layout->addLayout(n_max_distance_layout,0);
    }

    {
        QHBoxLayout * n_iterations_layout = new QHBoxLayout;
        // Rmin
        n_iterations_layout->addWidget(new QLabel("Number of seeding iterations: "),0);
        n_iterations_layout->addWidget(m_n_seeding_iterations,0);
        // Padding
        n_iterations_layout->addWidget(new QLabel(""),1);

        layout->addLayout(n_iterations_layout,0);
    }

    {
        QHBoxLayout * n_equidistant_layout = new QHBoxLayout;
        // Rmin
        n_equidistant_layout->addWidget(new QLabel("Equidistant seeds: "),0);
        n_equidistant_layout->addWidget(m_equidistant_cb,0);
        // Padding
        n_equidistant_layout->addWidget(new QLabel(""),1);

        layout->addLayout(n_equidistant_layout,0);
    }

    {

        QHBoxLayout * point_size_layout = new QHBoxLayout;
        // Rmin
        point_size_layout->addWidget(new QLabel("Point sizes: from "),0);
        point_size_layout->addWidget(m_size_min_le,0);
        point_size_layout->addWidget(new QLabel(" to "),0);
        point_size_layout->addWidget(m_size_max_le,0);
        // Padding
        point_size_layout->addWidget(new QLabel(""),1);

        layout->addLayout(point_size_layout,0);
    }

    // Padding
    QHBoxLayout * padding_layout = new QHBoxLayout;
    padding_layout->addWidget(new QLabel(""),1, Qt::AlignCenter);
    layout->addLayout(padding_layout,1);

    // Start button
    {
        QHBoxLayout * ok_btn_layout = new QHBoxLayout;
        ok_btn_layout->addWidget(m_ok_btn,0, Qt::AlignCenter);
        layout->addLayout(ok_btn_layout,0);
    }

    setLayout(layout);
}

int ClusteredDistributionProducerDialog::getNSeedPoints()
{
    return m_n_seed_points->text().toInt();
}

int ClusteredDistributionProducerDialog::getNSeedingIterations()
{
    return m_n_seeding_iterations->text().toInt();
}

int ClusteredDistributionProducerDialog::getMaxSeedingDistance()
{
    return m_max_seeding_distance->text().toInt();
}

bool ClusteredDistributionProducerDialog::equidistantSeeding()
{
    return m_equidistant_cb->isChecked();
}

int ClusteredDistributionProducerDialog::getMinimumRadius()
{
    return m_size_min_le->text().toInt();
}

int ClusteredDistributionProducerDialog::getMaximumRadius()
{
    return m_size_max_le->text().toInt();
}
