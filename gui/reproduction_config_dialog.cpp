#include "reproduction_config_dialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QTabWidget>
#include <QCheckBox>

#include "../utils/file_utils.h"
#include "ecotracker/gui/ecodata_entries_widget.h"

/*********************
 * DIRSELECTORWIDGET *
 *********************/
DirSelectorWidget::DirSelectorWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f), m_ready(false)
{

}

DirSelectorWidget::~DirSelectorWidget()
{

}

bool DirSelectorWidget::isReady()
{
    return m_ready;
}

void DirSelectorWidget::setReady(bool ready)
{
    m_ready = ready;
}

/**************************
 * DATABASE SELECTION TAB *
 **************************/
DatabaseSelectorTab::DatabaseSelectorTab(QWidget * parent, Qt::WindowFlags f) :
    DirSelectorWidget(parent, f),
    m_db_entries_widget(new EcodataEntriesWidget)
{
    connect(m_db_entries_widget, SIGNAL(row_selected(int)), this, SLOT(selected_entry_changed(int)));

    init_layout();
}

DatabaseSelectorTab::~DatabaseSelectorTab()
{

}

void DatabaseSelectorTab::init_layout()
{
    QVBoxLayout * v_layout = new QVBoxLayout;
    v_layout->addWidget(m_db_entries_widget);

    setLayout(v_layout);
}

QString DatabaseSelectorTab::dir()
{
    return m_db_entries_widget->getDirOfSelectedEntry();
}

void DatabaseSelectorTab::selected_entry_changed(int row)
{
    setReady(true);
    m_selected_row = row;
    emit ready(true);
}

/**************************
 * EXPLICIT SELECTION TAB *
 **************************/
ExplicitSelectorTab::ExplicitSelectorTab(QWidget * parent, Qt::WindowFlags f) :
    DirSelectorWidget(parent, f),
    m_active_directory_te(new QLineEdit(this)),
    m_change_active_directory_btn(new QPushButton("...",this)),
    m_invalid_dir_label(new QLabel(this))
{
    m_invalid_dir_label->setStyleSheet("QLabel { color : red; }");
    m_invalid_dir_label->setText("Invalid directory!");
    m_invalid_dir_label->setVisible(false);

    // Active directory
    m_active_directory_te->setEnabled(false);
    connect(m_change_active_directory_btn, SIGNAL(clicked()), this, SLOT(change_active_directory()));

    init_layout();
}

ExplicitSelectorTab::~ExplicitSelectorTab()
{

}

void ExplicitSelectorTab::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    QHBoxLayout * active_directory_layout = new QHBoxLayout;
    active_directory_layout->addWidget(new QLabel("Active directory: "),0);
    active_directory_layout->addWidget(m_active_directory_te,1);
    active_directory_layout->addWidget(m_change_active_directory_btn,0);

    layout->addLayout(active_directory_layout,0);

    layout->addWidget(m_invalid_dir_label, 0, Qt::AlignCenter);

    layout->addWidget(new QLabel, 1);//Padding

    setLayout(layout);
}

void ExplicitSelectorTab::change_active_directory()
{
    QString active_dir = QFileDialog::getExistingDirectory(this,
        tr("Set active directory"), m_active_directory_te->text());

    if(active_dir != QString::null)
    {
        m_active_directory_te->setText(active_dir);
        if(FileUtils::check_directory_structure(active_dir))
        {
            setReady(true);
            m_invalid_dir_label->setVisible(false);
            emit ready(true);
        }
        else
        {
            setReady(false);
            m_invalid_dir_label->setVisible(true);
            emit ready(false);
        }
    }
}

QString ExplicitSelectorTab::dir()
{
    return m_active_directory_te->text();
}


/***********************
 * FILE CHOOSER WIDGET *
 ***********************/
FileChooserWidget::FileChooserWidget(QString heading, QString filter, QWidget * parent) :
    QWidget(parent),
    m_filename_te(new QLineEdit(this)),
    m_filechooser_btn( new QPushButton("...", this)),
    m_filter(filter)
{
    QBoxLayout * layout (new QHBoxLayout);
    layout->addWidget(new QLabel(heading, this));
    layout->addWidget(m_filename_te,1);
    layout->addWidget(m_filechooser_btn,0);
    setLayout(layout);

    connect(m_filechooser_btn, SIGNAL(clicked(bool)), this, SLOT(change_file()));

    setValid(false);
}

FileChooserWidget::~FileChooserWidget()
{

}

void FileChooserWidget::change_file()
{
    QString current_output_dir ( QFileInfo(m_filename_te->text()).absoluteDir().absolutePath() ) ;

    QString output_file ( QFileDialog::getSaveFileName(this,
        tr("Set output file"), current_output_dir, m_filter) );

    if(output_file != QString::null)
    {
        setValid(true);
        m_filename_te->setText(output_file);
        emit selected_file_changed();

    }
}

QString FileChooserWidget::filename()
{
    return m_filename_te->text();
}

bool FileChooserWidget::valid()
{
    return m_valid;
}

void FileChooserWidget::setValid(bool valid)
{
    m_valid = valid;
    if(!valid)
        m_filename_te->setStyleSheet("QLineEdit { background: red; }");
    else
        m_filename_te->setStyleSheet("");
}

/**********************************************
 * REPRODUCTION CONFIGURATION PRODUCER DIALOG *
 **********************************************/
#define N_ITERATIONS_DEFAULT 10000
#define OUTPUT_WIDTH_DEFAULT 500
#define OUTPUT_HEIGHT_DEFAULT 500
ReproductionConfigurationProducerDialog::ReproductionConfigurationProducerDialog() :
    m_db_selector_tab(new DatabaseSelectorTab(this)),
    m_explicit_selector_tab(new ExplicitSelectorTab(this)),
    m_tabs(new QTabWidget(this)),
    m_generate_img_checkbox(new QCheckBox(this)),
    m_n_iterations_te(new QLineEdit),
    m_output_w_te(new QLineEdit),
    m_output_h_te(new QLineEdit),
    m_ok_btn(new QPushButton("Start")),
    m_cancel_btn(new QPushButton("Cancel")),
    m_output_file_chooser(new FileChooserWidget("Output file: ", "Plantfiles (*.pf)", this)),
    m_output_img_file_chooser(new FileChooserWidget("Image file: ", "Images (*.jpg)", this))
{
    setModal(true);

    // Iterations
    m_n_iterations_te->setText(QString::number(N_ITERATIONS_DEFAULT));

    // Output size
    m_output_w_te->setText(QString::number(OUTPUT_WIDTH_DEFAULT));
    m_output_h_te->setText(QString::number(OUTPUT_HEIGHT_DEFAULT));

    // OK button
    m_ok_btn->setEnabled(false);
//    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(print_config()));

    // Cancel button
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(close()));

    // TABS
    m_tabs->addTab(m_db_selector_tab, "Database");
    m_tabs->addTab(m_explicit_selector_tab, "Explicit");
    connect(m_db_selector_tab, SIGNAL(ready(bool)), this, SLOT(validity_check()));
    connect(m_explicit_selector_tab, SIGNAL(ready(bool)), this, SLOT(validity_check()));
    connect(m_tabs, SIGNAL(currentChanged(int)), this, SLOT(tab_changed()));

    // Output selector
    connect(m_output_file_chooser, SIGNAL(selected_file_changed()), this, SLOT(validity_check()));

    // Output image selector
    connect(m_output_img_file_chooser, SIGNAL(selected_file_changed()), this, SLOT(validity_check()));

    // Image selector checkbox
    connect(m_generate_img_checkbox, SIGNAL(toggled(bool)), this, SLOT(img_cb_state_changed(bool)));
    m_generate_img_checkbox->setChecked(false);
    img_cb_state_changed(false);

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

    // Output file
    {
        layout->addWidget(m_output_file_chooser,0);
    }

    // Output image file checkbox
    {
        QHBoxLayout * img_cb_layout = new QHBoxLayout;
        img_cb_layout->addWidget(m_generate_img_checkbox, 0, Qt::AlignLeft);
        img_cb_layout->addWidget(new QLabel("Generate output image", this),0, Qt::AlignLeft);
        img_cb_layout->addWidget(new QLabel("", this),1, Qt::AlignLeft); //Padding
        layout->addLayout(img_cb_layout, 0);
    }

    // Output image file chooser
    {
        layout->addWidget(m_output_img_file_chooser,0);
    }

    // TABS
    layout->addWidget(m_tabs, 1);
//    QHBoxLayout * padding_layout = new QHBoxLayout;
//    padding_layout->addWidget(new QLabel(""),1, Qt::AlignCenter);
//    layout->addLayout(padding_layout,1);

    // Start button
    {
        QHBoxLayout * btn_layout = new QHBoxLayout;
        btn_layout->addWidget(m_cancel_btn,0, Qt::AlignCenter);
        btn_layout->addWidget(m_ok_btn,0, Qt::AlignCenter);
        layout->addLayout(btn_layout,0);
    }
    setLayout(layout);
}

QString ReproductionConfigurationProducerDialog::getOutputFilename()
{
    return m_output_file_chooser->filename();
}

void ReproductionConfigurationProducerDialog::tab_changed()
{
    validity_check();
}

void ReproductionConfigurationProducerDialog::img_cb_state_changed(bool checked)
{
    m_output_img_file_chooser->setEnabled(checked);
    m_output_img_file_chooser->setVisible(checked);
    validity_check();
}

void ReproductionConfigurationProducerDialog::validity_check()
{
    bool valid(true);
    valid &= static_cast<DirSelectorWidget*>(m_tabs->currentWidget())->isReady(); // Selected tab is valid directory
    valid &= (m_output_file_chooser->valid()); // Valid output file chosen
    valid &= (!m_generate_img_checkbox->isChecked() || m_output_img_file_chooser->valid());
    m_ok_btn->setEnabled(valid);
}

#include <QDebug>
void ReproductionConfigurationProducerDialog::print_config()
{
    qCritical() << "Active dir: " << static_cast<DirSelectorWidget*>(m_tabs->currentWidget())->dir();
    qCritical() << "Width: " << m_output_w_te->text().toInt();
    qCritical() << "Height: " << m_output_h_te->text().toInt();
    qCritical() << "Iterations: " << m_n_iterations_te->text().toInt();
}

ReproductionConfiguration ReproductionConfigurationProducerDialog::getConfiguration()
{
    return ReproductionConfiguration(
                static_cast<DirSelectorWidget*>(m_tabs->currentWidget())->dir(),
                m_output_w_te->text().toInt(),
                m_output_h_te->text().toInt(),
                m_n_iterations_te->text().toInt());
}
