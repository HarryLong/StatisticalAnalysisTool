#include "utils.h"
#include <malloc.h>
#include <algorithm>
#include <iostream>
#include <QImage>
#include "../analysis_point.h"
#include "point_drawer.h"

#include <QPainter>

/*****************************
 * RADIAL DISTRIBUTION UTILS *
 *****************************/
int RadialDistributionUtils::getRBracket(double distance, int r_min, int r_diff)
{
//    if(distance < 1)
//        return 0;
//    if(r_min == 0)
//        return 1 + ((int)((distance-1)/r_diff)) * r_diff;
    return r_min + ((int)((distance-r_min)/r_diff)) * r_diff;
}

/***********************
 * PROBABILISTIC UTILS *
 ***********************/
// DiceRoller must be set to return a value between 0 and 1000
bool ProbabilisticUtils::returnTrueWithProbability(float probability, DiceRoller & dice_roller)
{
    int i_probability(probability*1000);

    return (dice_roller.generate() < i_probability);
}

/**************
 * GEOM UTILS *
 **************/
float GeometricUtils::getCircleArea(int radius)
{
    return M_PI * (radius*radius);
}

/***************
 * IMAGE UTILS *
 ***************/
void ImageUtils::printPointsToImg(std::string image_file, const std::map<int,std::vector<AnalysisPoint> > & points, int width, int height)
{
    PointDrawer drawer(width, height);

    for(auto category_it( points.begin() ); category_it != points.end(); category_it++) // Draw higher priority first to ensure no overlap
    {
        for(auto point_it(category_it->second.begin()); point_it != category_it->second.end(); point_it++)
        {
            drawer.drawPoint(*point_it);
        }
    }

    drawer.toImage().save(QString(image_file.c_str()));
}

//int main(int argc, char *argv[])
//{
//    int total(500000);
//    int trues(0);

//    DiceRoller roller(0,1000);

//    for(int i = 0; i < total; i++)
//    {
//        if(ProbabilisticUtils::returnTrueWithProbability(0.5f, roller))
//            trues++;
//    }
//    std::cout << "Trues: " << trues << " / " << total << std::endl;
//    return 0;
//}


