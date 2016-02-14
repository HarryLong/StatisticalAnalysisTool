#include "reproduction_config_dialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QTabWidget>

/**********************************************
 * REPRODUCTION CONFIGURATION PRODUCER DIALOG *
 **********************************************/
#define N_ITERATIONS_DEFAULT 10000
#define OUTPUT_WIDTH_DEFAULT 500
#define OUTPUT_HEIGHT_DEFAULT 500
#define DEFAULT_OUTPUT_FILE "/home/harry/Pictures/radial_distribution_out.jpg"
#define DEFAULT_ACTIVE_DIR "/home/harry/.ecotracker/"
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

    // OK button
    m_ok_btn = new QPushButton("Start");
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));

    init_layout();
    setMinimumSize(1000,800);
}

ReproductionConfigurationProducerDialog::~ReproductionConfigurationProducerDialog()
{

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
    {
         // First set the active dir text
        m_active_directory_te->setText(active_dir);
        if(true)// Check whether this is itself a valid directory
        {

        }
        else // Check if this is a working directory with valid subdirectories
        {
            // Use DB to load stuff and check validity also.
        }

    }
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
    return ReproductionConfiguration(
                m_active_directory_te->text(),
                m_output_w_te->text().toInt(),
                m_output_h_te->text().toInt(),
                m_n_iterations_te->text().toInt());
}
