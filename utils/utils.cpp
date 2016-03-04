#include "utils.h"
#include <malloc.h>
#include <algorithm>
#include <iostream>
#include <QImage>
#include "../analysis_point.h"
#include "point_drawer.h"

#include <QPainter>
#include <QDebug>


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
    std::vector<PointDrawer> point_drawers;
    for(int i(0); i < points.size(); i++)
    {
        point_drawers.push_back(PointDrawer(width,height));
    }
    PointDrawer all_points_drawer(width, height);

    std::vector<int> ordered_category_ids;
    int i(0);
    for(auto category_it( points.rbegin() ); category_it != points.rend(); category_it++, i++) // Draw higher priority first to ensure no overlap
    {
        std::cout << "Printing points of category " << category_it->first << std::endl;
        ordered_category_ids.push_back(category_it->first);
        PointDrawer & category_drawer(point_drawers.at(i));
        for(auto point_it(category_it->second.begin()); point_it != category_it->second.end(); point_it++)
        {
            all_points_drawer.drawPoint(*point_it);
            category_drawer.drawPoint(*point_it);
        }
    }
    for(int i(0); i < points.size(); i++)
    {
        QString filename(QString::fromStdString(image_file));
        filename.append("_").append(QString::number(ordered_category_ids.at(i))).append(".jpg");
        point_drawers.at(i).toImage().save(filename);
        qCritical() << "Written file: " << filename;
    }
    QString img_file(QString::fromStdString(image_file));
    if(!img_file.endsWith(".jpg"))
        img_file.append(".jpg");
    all_points_drawer.toImage().save(img_file);
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


