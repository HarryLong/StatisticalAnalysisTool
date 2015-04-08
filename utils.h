#ifndef UTILS_H
#define UTILS_H

#include "dice_roller.h"
#include <QPoint>

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
class AnalysisPoint;
namespace FileUtils{
    void printPointsToImg(std::string image_file, const std::vector<AnalysisPoint*> & points, int width, int height);
}

#endif //UTILS_H
