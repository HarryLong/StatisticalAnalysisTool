#ifndef UTILS_H
#define UTILS_H

#include "dice_roller.h"
#include <QPoint>
#include <QString>
#include <map>

/*************
 * BIN UTILS *
 *************/
namespace Binutils{
    unsigned char* toBin(unsigned int value, int n_bytes = 4);
    unsigned char* toBin(float value, int n_bytes = 4);

    int readInt32(unsigned char * data, int n_bytes = 4);
    float readFloat32(unsigned char * data, int n_bytes = 4);

    void insertPadding(unsigned char * data, int from, int to);
}

/*****************************
 * RADIAL DISTRIBUTION UTILS *
 *****************************/
namespace RadialDistributionUtils{
    int getRBracket(double distance, int r_min, int r_diff);
}

/***********************
 * PROBABILISTIC UTILS *
 ***********************/
namespace ProbabilisticUtils{
    bool returnTrueWithProbability(float probability, DiceRoller & dice_roller);
}

/**************
 * FILE UTILS *
 **************/
namespace FileUtils{
    bool init_directory_structure(QString directory, QString & radial_distribution_folder, QString & category_properties_folder,
                              QString & csv_files_folder);
    bool check_directory_structure(QString directory);
    std::vector<QString> get_radial_distribution_files(QString directory);
    std::vector<QString> get_category_properties_files(QString directory);
    QString get_configuration_file(QString directory);
}

/***************
 * IMAGE UTILS *
 ***************/
class AnalysisPoint;
namespace ImageUtils{
    void printPointsToImg(std::string image_file, const std::map<int,std::vector<AnalysisPoint*> > & points, int width, int height);
}

#endif //UTILS_H
