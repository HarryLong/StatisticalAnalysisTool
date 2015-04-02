#include "dialogs.h"
#include <algorithm>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>

#define R_MIN_DEFAULT 0
#define R_DIFF_DEFAULT 5
#define R_ITERATIONS_DEFAULT 1000
#define R_OUTPUT_WIDTH_DEFAULT 500
#define R_OUTPUT_HEIGHT_DEFAULT 500

#define R_MIN_RANGE_MIN 0
#define R_MIN_RANGE_MAX 100

#define R_MAX_RANGE_MIN 10

#define R_DIFF_RANGE_MIN 1
#define R_DIFF_RANGE_MAX 200

#define DEFAULT_RAD_FILE_LOCATION "/home/harry/radial_distribution_app/output_distribution.rad"
#define DEFAULT_CSV_FILE_LOCATION "/home/harry/radial_distribution_app/output_distribution.csv"
#define DEFAULT_IMG_FILE_LOCATION "/home/harry/radial_distribution_app/output_distribution.jpg"

/****************************************
 * RADIAL DISTSRIBUTION PRODUCER DIALOG *
 ****************************************/
RadialDistributionProducerDialog::RadialDistributionProducerDialog()
{
    setModal(true);
    m_r_min_sb = new QSpinBox;
    m_r_min_sb->setRange(R_MIN_RANGE_MIN, R_MIN_RANGE_MAX);
    m_r_min_sb->setValue(R_MIN_DEFAULT);

    m_r_max_sb = new QSpinBox;

    m_r_diff_sb = new QSpinBox;
    m_r_diff_sb->setRange(R_DIFF_RANGE_MIN, R_DIFF_RANGE_MAX);
    m_r_diff_sb->setValue(R_DIFF_DEFAULT);

    m_n_iterations_te = new QLineEdit;
    m_n_iterations_te->setText(QString::number(R_ITERATIONS_DEFAULT));

    m_output_w_te = new QLineEdit;
    m_output_w_te->setText(QString::number(R_OUTPUT_WIDTH_DEFAULT));

    m_output_h_te = new QLineEdit;
    m_output_h_te->setText(QString::number(R_OUTPUT_HEIGHT_DEFAULT));

    m_output_rad_file_te = new QLineEdit;
    m_output_rad_file_te->setEnabled(false);
    m_output_rad_file_te->setText(DEFAULT_RAD_FILE_LOCATION);

    m_output_csv_file_te = new QLineEdit;
    m_output_csv_file_te->setEnabled(false);
    m_output_csv_file_te->setText(DEFAULT_CSV_FILE_LOCATION);

    m_output_img_file_te = new QLineEdit;
    m_output_img_file_te->setEnabled(false);
    m_output_img_file_te->setText(DEFAULT_IMG_FILE_LOCATION);

    m_ok_btn = new QPushButton("Start");
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));

    // Actions
    m_change_rad_file_btn = new QPushButton("...");
    connect(m_change_rad_file_btn, SIGNAL(clicked()), this, SLOT(change_rad_file_location()));

    m_change_csv_file_btn = new QPushButton("...");
    connect(m_change_csv_file_btn, SIGNAL(clicked()), this, SLOT(change_csv_file_location()));

    m_change_img_file_btn = new QPushButton("...");
    connect(m_change_img_file_btn, SIGNAL(clicked()), this, SLOT(change_img_file_location()));

    init_layout();
}

RadialDistributionProducerDialog::~RadialDistributionProducerDialog()
{
}

void RadialDistributionProducerDialog::setAnalysisArea(int width, int height)
{
    m_r_max_sb->setRange(R_MAX_RANGE_MIN, std::max(width, height));
    m_r_max_sb->setValue(std::max(width, width)/2.0f);
}

void RadialDistributionProducerDialog::init_layout()
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
        iterations_layout->addWidget(m_n_iterations_te,1);
//        // Padding
//        iterations_layout->addWidget(new QLabel(""));

        layout->addLayout(iterations_layout,0);
    }

    // Output rad file
    {
        QHBoxLayout * output_rad_file_layout = new QHBoxLayout;
        output_rad_file_layout->addWidget(new QLabel("Output rad file: "),0);
        output_rad_file_layout->addWidget(m_output_rad_file_te,1);
        output_rad_file_layout->addWidget(m_change_rad_file_btn,0);
        layout->addLayout(output_rad_file_layout,0);
    }

    // Output csv file
    {
        QHBoxLayout * output_csv_file_layout = new QHBoxLayout;
        output_csv_file_layout->addWidget(new QLabel("Output csv file: "),0);
        output_csv_file_layout->addWidget(m_output_csv_file_te,1);
        output_csv_file_layout->addWidget(m_change_csv_file_btn,0);
        layout->addLayout(output_csv_file_layout,0);
    }

    // Output img file
    {
        QHBoxLayout * output_img_file_layout = new QHBoxLayout;
        output_img_file_layout->addWidget(new QLabel("Output image file: "),0);
        output_img_file_layout->addWidget(m_output_img_file_te,1);
        output_img_file_layout->addWidget(m_change_img_file_btn,0);
        layout->addLayout(output_img_file_layout,0);
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

void RadialDistributionProducerDialog::change_rad_file_location()
{
    QString rad_file_name = QFileDialog::getSaveFileName(this,
        tr("Set output radial distribution file"), m_output_rad_file_te->text(), tr("Rad files(*.rad)"));

    if(rad_file_name != QString::null)
    {
        if(!rad_file_name.endsWith(".rad"))
            rad_file_name.append(".rad");
        m_output_rad_file_te->setText(rad_file_name);
    }
}

void RadialDistributionProducerDialog::change_csv_file_location()
{
    QString csv_file_name = QFileDialog::getSaveFileName(this,
        tr("Set output csv file"), m_output_csv_file_te->text(), tr("CSV files(*.csv)"));

    if(csv_file_name != QString::null)
    {
        if(!csv_file_name.endsWith(".csv"))
            csv_file_name.append(".csv");
        m_output_csv_file_te->setText(csv_file_name);
    }
}

void RadialDistributionProducerDialog::change_img_file_location()
{
    QString img_file_name = QFileDialog::getSaveFileName(this,
        tr("Set output img file"), m_output_img_file_te->text(), tr("JPG files(*.jpg)"));

    if(img_file_name != QString::null)
    {
        if(!img_file_name.endsWith(".jpg"))
            img_file_name.append(".jpg");
        m_output_img_file_te->setText(img_file_name);
    }
}

ReproductionSettings RadialDistributionProducerDialog::getReproductionSettings()
{
    return ReproductionSettings(
                m_r_min_sb->value(),
                m_r_max_sb->value(),
                m_r_diff_sb->value(),
                m_output_w_te->text().toInt(),
                m_output_h_te->text().toInt(),
                m_n_iterations_te->text().toInt(),
                m_output_rad_file_te->text(),
                m_output_csv_file_te->text(),
                m_output_img_file_te->text()
                );
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

    m_ok_btn = new QPushButton("Start");
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
        n_points_layout->addWidget(new QLabel("Number of points: "));
        n_points_layout->addWidget(m_n_points,0, Qt::AlignLeft);
        // Padding
        n_points_layout->addWidget(new QLabel(""));

        layout->addLayout(n_points_layout,0);
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

int RandomDistributionProducerDialog::getNPoints()
{
    return m_n_points->text().toInt();
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

    m_ok_btn = new QPushButton("Start");
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
        n_seeding_points_layout->addWidget(new QLabel("Number of seeding points: "));
        n_seeding_points_layout->addWidget(m_n_seed_points,0, Qt::AlignLeft);
        // Padding
        n_seeding_points_layout->addWidget(new QLabel(""));

        layout->addLayout(n_seeding_points_layout,0);
    }

    {
        QHBoxLayout * n_max_distance_layout = new QHBoxLayout;
        // Rmin
        n_max_distance_layout->addWidget(new QLabel("Max seeding distance: "));
        n_max_distance_layout->addWidget(m_max_seeding_distance,0, Qt::AlignLeft);
        // Padding
        n_max_distance_layout->addWidget(new QLabel(""));

        layout->addLayout(n_max_distance_layout,0);
    }

    {
        QHBoxLayout * n_iterations_layout = new QHBoxLayout;
        // Rmin
        n_iterations_layout->addWidget(new QLabel("Number of seeding iterations: "));
        n_iterations_layout->addWidget(m_n_seeding_iterations,0, Qt::AlignLeft);
        // Padding
        n_iterations_layout->addWidget(new QLabel(""));

        layout->addLayout(n_iterations_layout,0);
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
