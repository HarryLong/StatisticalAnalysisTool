#include "dependency_analyzer.h"
#include "../analysis_point.h"
#include "../point_spatial_hashmap.h"

#include <QLineF>

#include <QDebug>

DependencyAnalyzer::Dependencies DependencyAnalyzer::getDependencies(std::map<int, std::vector<AnalysisPoint>> points, int analysis_window_width, int analysis_window_height)
{
    // First add all points to spatial hashmap
    PointSpatialHashmap spatial_point_storage(analysis_window_width, analysis_window_height);
    Dependencies dependencies;
    for(auto category_it(points.begin()); category_it != points.end(); category_it++)
    {
        dependencies.emplace(category_it->first, std::set<int>());
        for(auto point_it(category_it->second.begin()); point_it != category_it->second.end(); point_it++)
        {
            spatial_point_storage.addPoint(*point_it);
        }
    }

    // Iterate through each point and check whether its category is dependent on others
    for(auto category_it(points.begin()); category_it != points.end(); category_it++)
    {
        int category_id(category_it->first);
        for(auto point_it(category_it->second.begin()); point_it != category_it->second.end(); point_it++)
        {
            AnalysisPoint & point(*point_it);
            std::vector<AnalysisPoint> potential_points_which_shade_this_one (spatial_point_storage.getPossibleReachablePoints(point, 36)); // 36 as the ecosystem simulator window is 25by25(36 across)
            int point_which_shades_this_one_category_id(-1);
            for(AnalysisPoint & p : potential_points_which_shade_this_one)
            {
                if(p.getRadius() > 1)
                {
                    QLineF distance_measure(QPoint(point.getCenter().x(), point.getCenter().y()),
                                           QPoint(p.getCenter().x(), p.getCenter().y()));
//                    if(category_id == 6)
//                    {
//                        qCritical() << "Distance: " << (distance_measure.length()-36-p.getRadius());
//                        qCritical() << "Source point: [" << point.getCenter().x() << ", " << point.getCenter().y() << "] R: " << point.getRadius();
//                        qCritical() << "Destination point: [" << p.getCenter().x() << ", " << p.getCenter().y() << "] R: " << p.getRadius();

//                    }
                    if(distance_measure.length() - 36 <= p.getRadius()) // 25 is the buffer cell width of the ecosystem simulator
                    {
                        point_which_shades_this_one_category_id = p.getCategoryId();
                        break;
                    }
                }
            }
            if(point_which_shades_this_one_category_id != -1) // point is not shaded
                dependencies[category_id].insert(point_which_shades_this_one_category_id);
            else
            {
                dependencies.erase(category_id);
                break;
            }
        }
    }

    return dependencies;
}
