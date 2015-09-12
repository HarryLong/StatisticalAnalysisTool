#ifndef UTILS_H
#define UTILS_H

#include "dice_roller.h"
#include <QPoint>
#include <QString>
#include <map>

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

namespace GeometricUtils{
    float getCircleArea(int radius);
}

/***************
 * IMAGE UTILS *
 ***************/
class AnalysisPoint;
namespace ImageUtils{
    void printPointsToImg(std::string image_file, const std::map<int,std::vector<AnalysisPoint> > & points, int width, int height);
}

#endif //UTILS_H
