#include "dialogs.h"
#include <algorithm>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>

/**********************************************
 * REPRODUCTION CONFIGURATION PRODUCER DIALOG *
 **********************************************/
#define N_ITERATIONS_DEFAULT 10000
#define OUTPUT_WIDTH_DEFAULT 500
#define OUTPUT_HEIGHT_DEFAULT 500
#define DEFAULT_OUTPUT_FILE "/home/harry/Pictures/radial_distribution_out.jpg"
#define DEFAULT_ACTIVE_DIR "/home/harry/radial_distribution_app/"
ReproductionConfigurationProducerDialog::ReproductionConfigurationProducerDialog()
{
    setModal(true);

    // Iterations
    m_n_iterations_te = new QLineEdit;
    m_n_iterations_te->setText(QString::number(N_ITERATIONS_DEFAULT));

    // Output size
    m_output_w_te = new QLineEdit;
    m_output_w_te->setText(QString::number(OUTPUT_WIDTH_DEFAULT));

    m_output_h_te = new QLineEdit;
    m_output_h_te->setText(QString::number(OUTPUT_HEIGHT_DEFAULT));

    // Active directory
    m_active_directory_te = new QLineEdit;
    m_active_directory_te->setEnabled(false);
    m_active_directory_te->setText(DEFAULT_ACTIVE_DIR);
    m_change_active_directory_btn = new QPushButton("...", this);
    connect(m_change_active_directory_btn, SIGNAL(clicked()), this, SLOT(change_active_directory()));

    // Output file
    m_output_file_te = new QLineEdit;
    m_output_file_te->setEnabled(false);
    m_output_file_te->setText(DEFAULT_OUTPUT_FILE);
    m_change_output_file_btn = new QPushButton("...", this);
    connect(m_change_output_file_btn, SIGNAL(clicked()), this, SLOT(change_output_file()));

    // Initialization types
    m_init_type_btn_group = new QButtonGroup;

    m_init_two_point_rb = new QRadioButton;
    m_init_type_btn_group->addButton(m_init_two_point_rb);
    m_init_match_density_rb = new QRadioButton;
    m_init_type_btn_group->addButton(m_init_match_density_rb);
    m_init_two_point_rb->setChecked(true);

    // Generation algo
    m_generation_algo_btn_group = new QButtonGroup;
    m_birth_and_deaths_generation_algo_rb = new QRadioButton;
    m_generation_algo_btn_group->addButton(m_birth_and_deaths_generation_algo_rb);
    m_random_moves_generation_algo_rb = new QRadioButton;
    m_generation_algo_btn_group->addButton(m_random_moves_generation_algo_rb);
    m_birth_and_deaths_generation_algo_rb->setChecked(true);
    connect(m_generation_algo_btn_group, SIGNAL(buttonClicked(int)), this, SLOT(generation_algo_changed()));

    // OK button
    m_ok_btn = new QPushButton("Start");
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));

    init_layout();
}

ReproductionConfigurationProducerDialog::~ReproductionConfigurationProducerDialog()
{
    delete m_init_type_btn_group;
}

void ReproductionConfigurationProducerDialog::generation_algo_changed()
{
    if(m_random_moves_generation_algo_rb->isChecked())
    {
        m_init_match_density_rb->setChecked(true);
        for(QAbstractButton * btn : m_init_type_btn_group->buttons())
            btn->setEnabled(false);
    }
    else
    {
        for(QAbstractButton * btn : m_init_type_btn_group->buttons())
            btn->setEnabled(true);
    }
}

void ReproductionConfigurationProducerDialog::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    // Output dimension
    {
        QHBoxLayout * output_dim_layout = new QHBoxLayout;
        // Width
        output_dim_layout->addWidget(new QLabel("Output image: w: "),0);
        output_dim_layout->addWidget(m_output_w_te,0);
        // Height
        output_dim_layout->addWidget(new QLabel(" & h: "),0);
        output_dim_layout->addWidget(m_output_h_te,0);
        // Padding
        output_dim_layout->addWidget(new QLabel(""),1);

        layout->addLayout(output_dim_layout,0);
    }

    // Iterations
    {
        QHBoxLayout * iterations_layout = new QHBoxLayout;
        iterations_layout->addWidget(new QLabel("Iterations: "),0);
        iterations_layout->addWidget(m_n_iterations_te,0);
        // Padding
        iterations_layout->addWidget(new QLabel("")),1;

        layout->addLayout(iterations_layout,0);
    }

    // Initializtion type
    {
        QHBoxLayout * init_type_layout = new QHBoxLayout;

        init_type_layout->addWidget(new QLabel("Initialization type: "),0);
        // Two points
        init_type_layout->addWidget(m_init_two_point_rb,0);
        init_type_layout->addWidget(new QLabel("Two points  "),0);
        // Match density
        init_type_layout->addWidget(m_init_match_density_rb,0);
        init_type_layout->addWidget(new QLabel("Match density  "),0);
        // Padding
        init_type_layout->addWidget(new QLabel(""),1);

        layout->addLayout(init_type_layout,0);
    }


    // Generation algo
    {
        QHBoxLayout * generation_algo_layout = new QHBoxLayout;

        generation_algo_layout->addWidget(new QLabel("Generation algo: "),0);
        // Two points
        generation_algo_layout->addWidget(m_birth_and_deaths_generation_algo_rb,0);
        generation_algo_layout->addWidget(new QLabel("Iterative B & D  "),0);
        // Match density
        generation_algo_layout->addWidget(m_random_moves_generation_algo_rb,0);
        generation_algo_layout->addWidget(new QLabel("Random moves  "),0);
        // Padding
        generation_algo_layout->addWidget(new QLabel(""),1);

        layout->addLayout(generation_algo_layout,0);
    }

    // Active directory
    {
        QHBoxLayout * active_directory_layout = new QHBoxLayout;
        active_directory_layout->addWidget(new QLabel("Active directory: "),0);
        active_directory_layout->addWidget(m_active_directory_te,1);
        active_directory_layout->addWidget(m_change_active_directory_btn,0);
        layout->addLayout(active_directory_layout,0);
    }

    // Output file
    {
        QHBoxLayout * output_file_layout = new QHBoxLayout;
        output_file_layout->addWidget(new QLabel("Output file: "),0);
        output_file_layout->addWidget(m_output_file_te,1);
        output_file_layout->addWidget(m_change_output_file_btn,0);
        layout->addLayout(output_file_layout,0);
    }

    // Padding
    QHBoxLayout * padding_layout = new QHBoxLayout;
    padding_layout->addWidget(new QLabel(""),1, Qt::AlignCenter);
    layout->addLayout(padding_layout,1);

    // Start button
    {
        QHBoxLayout * padding_layout = new QHBoxLayout;
        padding_layout->addWidget(m_ok_btn,0, Qt::AlignCenter);
        layout->addLayout(padding_layout,0);
    }
    setLayout(layout);
}

void ReproductionConfigurationProducerDialog::change_active_directory()
{
    QString active_dir = QFileDialog::getExistingDirectory(this,
        tr("Set active directory"), m_active_directory_te->text());

    if(active_dir != QString::null)
        m_active_directory_te->setText(active_dir);
}

void ReproductionConfigurationProducerDialog::change_output_file()
{
    QString current_output_dir ( QFileInfo(m_output_file_te->text()).absoluteDir().absolutePath() ) ;

    QString output_file ( QFileDialog::getSaveFileName(this,
        tr("Set output file"), current_output_dir, tr("Images (*.jpg)")) );

    if(output_file != QString::null)
        m_output_file_te->setText(output_file);
}

QString ReproductionConfigurationProducerDialog::getOutputFile()
{
    return m_output_file_te->text();
}

ReproductionConfiguration ReproductionConfigurationProducerDialog::getConfiguration()
{
    ReproductionConfiguration::InitializationType init_type;
    if(m_init_match_density_rb->isChecked())
        init_type = ReproductionConfiguration::InitializationType::MatchDensity;
    else if(m_init_two_point_rb->isChecked())
        init_type = ReproductionConfiguration::InitializationType::TwoPoints;

    ReproductionConfiguration::GenerationAlgorithm generation_algo;
    if(m_birth_and_deaths_generation_algo_rb->isChecked())
        generation_algo = ReproductionConfiguration::GenerationAlgorithm::BirthsAndDeaths;
    else if(m_random_moves_generation_algo_rb->isChecked())
        generation_algo = ReproductionConfiguration::GenerationAlgorithm::RandomMoves;

    return ReproductionConfiguration(
                m_active_directory_te->text(),
                m_output_w_te->text().toInt(),
                m_output_h_te->text().toInt(),
                m_n_iterations_te->text().toInt(),
                init_type,
                generation_algo);
}

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
    m_output_dir_te->setText(DEFAULT_ACTIVE_DIR);

    // Change output dir button
    m_change_output_dir_btn = new QPushButton("...", this);
    connect(m_change_output_dir_btn, SIGNAL(clicked()), this, SLOT(change_output_directory()));

    // OK button
    m_ok_btn = new QPushButton("Start");
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
        m_output_dir_te->setText(output_dir);
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
