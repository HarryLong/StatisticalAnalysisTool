#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>

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
            QString output_img_file_location,
            QString output_size_properties_file_location,
            QString output_size_properties_csv_file_location,
            bool init_two_points,
            bool init_match_distribution,
            bool use_birth_and_death_genaration_algo,
            bool use_random_moves_generation_algo
            ) :
        r_min(r_min),
        r_max(r_max),
        r_diff(r_diff),
        output_img_w(output_img_w),
        output_img_h(output_img_h),
        n_iterations(n_iterations),
        output_rad_file_location(output_rad_file_location),
        output_csv_file_location(output_csv_file_location),
        output_img_file_location(output_img_file_location),
        output_size_properties_file_location(output_size_properties_file_location),
        output_size_properties_csv_file_location(output_size_properties_csv_file_location),
        init_two_points(init_two_points),
        init_match_distribution(init_match_distribution),
        use_birth_and_death_genaration_algo(use_birth_and_death_genaration_algo),
        use_random_moves_generation_algo(use_random_moves_generation_algo)    {}

    const int r_min;
    const int r_max;
    const int r_diff;

    const int output_img_w;
    const int output_img_h;

    const int n_iterations;

    const QString output_rad_file_location;
    const QString output_csv_file_location;
    const QString output_size_properties_file_location;
    const QString output_size_properties_csv_file_location;
    const QString output_img_file_location;

    bool init_two_points;
    bool init_match_distribution;

    bool use_birth_and_death_genaration_algo;
    bool use_random_moves_generation_algo;
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
    void change_rad_csv_file_location();
    void change_img_file_location();
    void change_size_properties_file_location();
    void change_size_properties_csv_file_location();
    void generation_algo_changed();

private:
    void init_layout();

    QSpinBox * m_r_min_sb;
    QSpinBox * m_r_max_sb;
    QSpinBox * m_r_diff_sb;

    QLineEdit * m_output_w_te;
    QLineEdit * m_output_h_te;
    QLineEdit * m_n_iterations_te;

    QLineEdit * m_output_rad_file_te;
    QLineEdit * m_output_rad_csv_file_te;
    QLineEdit * m_output_point_size_file_te;
    QLineEdit * m_output_point_size_csv_file_te;
    QLineEdit * m_output_img_file_te;

    QPushButton * m_change_rad_file_btn;
    QPushButton * m_change_rad_csv_file_btn;
    QPushButton * m_change_point_size_file_btn;
    QPushButton * m_change_point_size_csv_file_btn;

    QPushButton * m_change_img_file_btn;

    QButtonGroup * m_init_type_btn_group;
    QRadioButton * m_init_two_point_rb;
    QRadioButton * m_init_match_density_rb;

    QButtonGroup * m_generation_algo_btn_group;
    QRadioButton * m_birth_and_deaths_generation_algo_rb;
    QRadioButton * m_random_moves_generation_algo_rb;

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
