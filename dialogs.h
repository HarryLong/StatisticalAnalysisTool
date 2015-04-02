#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

/****************************************
 * RADIAL DISTSRIBUTION PRODUCER DIALOG *
 ****************************************/
struct ReproductionSettings{
public:
    ReproductionSettings(
            int r_min,
            int r_max,
            int r_diff,
            int output_img_w,
            int output_img_h,
            int n_iterations,
            QString output_rad_file_location,
            QString output_csv_file_location,
            QString output_img_file_location
            ) :
        r_min(r_min),
        r_max(r_max),
        r_diff(r_diff),
        output_img_w(output_img_w),
        output_img_h(output_img_h),
        n_iterations(n_iterations),
        output_rad_file_location(output_rad_file_location),
        output_csv_file_location(output_csv_file_location),
        output_img_file_location(output_img_file_location) {}

    const int r_min;
    const int r_max;
    const int r_diff;

    const int output_img_w;
    const int output_img_h;

    const int n_iterations;

    const QString output_rad_file_location;
    const QString output_csv_file_location;
    const QString output_img_file_location;
};

class RadialDistributionProducerDialog : public QDialog
{
Q_OBJECT
public:
    RadialDistributionProducerDialog();
    ~RadialDistributionProducerDialog();

    void setAnalysisArea(int width, int height);
    ReproductionSettings getReproductionSettings();

private slots:
    void change_rad_file_location();
    void change_csv_file_location();
    void change_img_file_location();

private:
    void init_layout();

    QSpinBox * m_r_min_sb;
    QSpinBox * m_r_max_sb;
    QSpinBox * m_r_diff_sb;

    QLineEdit * m_output_w_te;
    QLineEdit * m_output_h_te;
    QLineEdit * m_n_iterations_te;

    QLineEdit * m_output_rad_file_te;
    QLineEdit * m_output_csv_file_te;
    QLineEdit * m_output_img_file_te;

    QPushButton * m_change_rad_file_btn;
    QPushButton * m_change_csv_file_btn;
    QPushButton * m_change_img_file_btn;

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

private:
    void init_layout();

    QLineEdit * m_n_points;

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

private:
    void init_layout();

    QLineEdit * m_n_seed_points;
    QLineEdit * m_n_seeding_iterations;
    QLineEdit * m_max_seeding_distance;

    QPushButton * m_ok_btn;
};


#endif
