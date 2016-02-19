#ifndef REPRODUCTION_CONFIG_DIALOG_H
#define REPRODUCTION_CONFIG_DIALOG_H

#include "../reproducer/reproduction_configuration.h"
#include <QDialog>
class QLineEdit;
class QPushButton;
class QTabWidget;
class QLabel;
class EcodataEntriesWidget;
class QCheckBox;

/*********************
 * DIRSELECTORWIDGET *
 *********************/
class DirSelectorWidget : public QWidget {
Q_OBJECT
public:
    DirSelectorWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~DirSelectorWidget();
    virtual QString dir() = 0;

    bool isReady();

signals:
    void ready(bool);

protected:
    void setReady(bool);

private:
    bool m_ready;
};

/**************************
 * DATABASE SELECTION TAB *
 **************************/
class DatabaseSelectorTab : public DirSelectorWidget
{
Q_OBJECT
public:
    DatabaseSelectorTab(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~DatabaseSelectorTab();
    QString dir();

private slots:
    void selected_entry_changed(int);

private:
    void init_layout();
    EcodataEntriesWidget * m_db_entries_widget;

    int m_selected_row;
};

/**************************
 * EXPLICIT SELECTION TAB *
 **************************/
class ExplicitSelectorTab : public DirSelectorWidget
{
Q_OBJECT
public:
    ExplicitSelectorTab(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ExplicitSelectorTab();

    QString dir();

private slots:
    void change_active_directory();

private:
    void init_layout();

    QLineEdit * m_active_directory_te;
    QPushButton * m_change_active_directory_btn;
    QLabel * m_invalid_dir_label;
};

/***********************
 * FILE CHOOSER WIDGET *
 ***********************/
class FileChooserWidget : public QWidget
{
Q_OBJECT
public:
    FileChooserWidget(QString heading, QString filter = "", QWidget * parent = 0);
    ~FileChooserWidget();
    QString filename();
    bool valid();

signals:
    void selected_file_changed();

private slots:
    void change_file();

private:
    void setValid(bool);
    QLineEdit * m_filename_te;
    QPushButton * m_filechooser_btn;
    QString m_filter;
    bool m_valid;
};

/**********************************************
 * REPRODUCTION CONFIGURATION PRODUCER DIALOG *
 **********************************************/
class ReproductionConfigurationProducerDialog : public QDialog
{
Q_OBJECT
public:
    ReproductionConfigurationProducerDialog();
    ~ReproductionConfigurationProducerDialog();

    void setAnalysisArea(int width, int height);
    ReproductionConfiguration getConfiguration();
    QString getOutputFilename();

    bool genImage();
    QString getOutputImageFilename();

private slots:
    void print_config();
    void tab_changed();
    void img_cb_state_changed(bool);
    void validity_check();

private:
    void init_layout();

//    enum DirSelectorTabs{
//        _DB = 0,
//        _EXPLICIT
//    };

    FileChooserWidget * m_output_file_chooser;
    FileChooserWidget * m_output_img_file_chooser;

    QLineEdit * m_output_w_te;
    QLineEdit * m_output_h_te;
    QLineEdit * m_n_iterations_te;

    QPushButton * m_ok_btn;
    QPushButton * m_cancel_btn;

    QTabWidget * m_tabs;
    DirSelectorWidget * m_db_selector_tab;
    DirSelectorWidget * m_explicit_selector_tab;

    QCheckBox * m_generate_img_checkbox;
};

#endif //REPRODUCTION_CONFIG_DIALOG_H
