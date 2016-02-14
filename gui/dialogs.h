#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>

#include "../analyser/analysis_configuration.h"


/******************************************
 * ANALYSIS CONFIGURATION PRODUCER DIALOG *
 ******************************************/
class AnalysisConfigurationProducerDialog : public QDialog
{
Q_OBJECT
public:
    AnalysisConfigurationProducerDialog();
    ~AnalysisConfigurationProducerDialog();

    void setAnalysisWindowDimensions(int width, int height);
    AnalysisConfiguration getConfiguration();
    QString getOutputDir();
    void setAnalysisArea(int width, int height);

private slots:
    void change_output_directory();

private:
    void init_layout();

    int m_analysis_window_width, m_analysis_window_height;

    QSpinBox * m_r_min_sb;
    QSpinBox * m_r_max_sb;
    QSpinBox * m_r_diff_sb;

    QLineEdit * m_output_dir_te;

    QPushButton * m_change_output_dir_btn;

    QPushButton * m_ok_btn;
};


/****************************************
 * RANDOM DISTSRIBUTION PRODUCER DIALOG *
 ****************************************/
class RandomDistributionProducerDialog : public QDialog
{
public:
    RandomDistributionProducerDialog();
    ~RandomDistributionProducerDialog();
    int getNPoints();
    int getMinimumRadius();
    int getMaximumRadius();

private:
    void init_layout();

    QLineEdit * m_n_points;

    QLineEdit * m_size_min_le;
    QLineEdit * m_size_max_le;

    QPushButton * m_ok_btn;
};


/*******************************************
 * CLUSTERED DISTSRIBUTION PRODUCER DIALOG *
 *******************************************/
class ClusteredDistributionProducerDialog : public QDialog
{
public:
    ClusteredDistributionProducerDialog();
    ~ClusteredDistributionProducerDialog();
    int getNSeedPoints();
    int getNSeedingIterations();
    int getMaxSeedingDistance();
    bool equidistantSeeding();
    int getMinimumRadius();
    int getMaximumRadius();

private:
    void init_layout();

    QLineEdit * m_n_seed_points;
    QLineEdit * m_n_seeding_iterations;
    QLineEdit * m_max_seeding_distance;

    QCheckBox * m_equidistant_cb;

    QLineEdit * m_size_min_le;
    QLineEdit * m_size_max_le;

    QPushButton * m_ok_btn;
};


#endif
