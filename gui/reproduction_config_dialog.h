#ifndef REPRODUCTION_CONFIG_DIALOG_H
#define REPRODUCTION_CONFIG_DIALOG_H

#include "ecotracker/gui/ecodata_entries_widget.h"
#include "../reproducer/reproduction_configuration.h"
#include <QDialog>
class QLineEdit;
class QPushButton;
class QTabWidget;

//class DatabaseSelectorTab : public QWidget
//{
//Q_OBJECT
//public:
//    DatabaseSelectorTab(QWidget * parent = 0, Qt::WindowFlags f = 0);
//    ~DatabaseSelectorTab();

//signals:
//    void ready(bool);

//private:
//    void init_layout();


//};

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
    QString getOutputFile();

private slots:
    void change_active_directory();
    void change_output_file();

private:
    void init_layout();

    QLineEdit * m_output_w_te;
    QLineEdit * m_output_h_te;
    QLineEdit * m_n_iterations_te;

    QLineEdit * m_output_file_te;
    QLineEdit * m_active_directory_te;

    QPushButton * m_change_active_directory_btn;
    QPushButton * m_change_output_file_btn;

    QPushButton * m_ok_btn;

    QTabWidget * m_tabs;
};

#endif //REPRODUCTION_CONFIG_DIALOG_H
