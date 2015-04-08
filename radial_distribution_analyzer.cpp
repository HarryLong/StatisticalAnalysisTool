#include "radial_distribution_analyzer.h"
#include <iostream>
#include <QLineF>
#include <math.h>
#include "utils.h"
#include "spatial_hashmap.h"

RadialDistributionAnalyzer::RadialDistributionAnalyzer(int r_min, int r_max, int r_diff) :
    r_min(r_min), r_max(r_max), r_diff(r_diff)
{
    if((r_max-r_min) % r_diff != 0)
    {
        std::cout << "R max increased from " << r_max << " to " << std::flush;
        r_max = r_min + ((((r_max-r_min)/r_diff) + 1) * r_diff);
        std::cout << r_max << std::endl;
    }
    // Calculate annular shell areas
    for(int r (r_min); r < r_max; r += r_diff)
    {
        double area ( (M_PI * pow(r+r_diff, 2)) - (M_PI * pow(r, 2)));
        m_annular_shell_areas.insert(std::pair<int,double>(r, area));
    }
}

RadialDistributionAnalyzer::~RadialDistributionAnalyzer()
{

}

#define SPATIAL_HASHMAP_CELL_WIDTH 10
#define SPATIAL_HASHMAP_CELL_HEIGHT 10
RadialDistribution RadialDistributionAnalyzer::getRadialDistribution(std::vector<AnalysisPoint*> reference_points, std::vector<AnalysisPoint*> target_points,
                                                                     int width, int height, int reference_points_id, int destination_points_id)
{
    // Optimization: build a spatial hashmap
    RadialDistributionSpatialHashmap spatial_point_storage(SPATIAL_HASHMAP_CELL_WIDTH, SPATIAL_HASHMAP_CELL_HEIGHT,
                                                          std::ceil(((float)width)/SPATIAL_HASHMAP_CELL_WIDTH),
                                                          std::ceil(((float)height)/SPATIAL_HASHMAP_CELL_HEIGHT));

    for(AnalysisPoint* p : target_points)
        spatial_point_storage.getCell(p->getCenter(), true)->points.push_back(p);

    int total_area( width * height );

    // First calculate the average density
    float avg_density (((float)target_points.size()) / total_area);

    // initialize the histogram and calculate different annular shell areas
    RadialDistribution::Histogram results;

    for(int r (r_min); r < r_max; r += r_diff)
        results.insert(std::pair<int,int>(r,0.0));

    float within_radius_distribution(.0f);

    double constant_normalization_factor(((float)width*height) / (reference_points.size()*target_points.size()));
    // Set histogram values
    QLineF line;
    for(AnalysisPoint * reference_point : reference_points)
    {
        line.setP1(reference_point->getCenter());

        int normalization_length(reference_point->getRadius()-1); // Perform analysis for each point normalized to a length of 1

        std::vector<SpatialHashMapCell*> cells_within_r_max(
                    spatial_point_storage.getCells(reference_point->getCenter(), r_max+spatial_point_storage.getCellWidth()+normalization_length)); // Add cell width so that it returns all cells within reach

        for(SpatialHashMapCell * cell : cells_within_r_max)
        {
            for(AnalysisPoint* target_point : cell->points)
            {
                if(reference_point != target_point)
                {
                    line.setP2(target_point->getCenter());
                    float length(std::max(0.0,line.length()-normalization_length));
                    if(length >= r_min && length < r_max)
                    {
                        int r_bracket(RadialDistributionUtils::getRBracket(length, r_min, r_diff));

                        if(RadialDistributionAnalyzer::overflows_border(reference_point->getCenter(), r_bracket, r_diff, width, height))
                        {
                            double small_circle_area(RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point->getCenter(), r_bracket, width, height));
                            double large_circle_area(RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point->getCenter(), r_bracket+r_diff, width, height));

                            double annular_shell_area (large_circle_area - small_circle_area);

                            results[r_bracket] += (constant_normalization_factor / annular_shell_area);
                        }
                        else
                        {
                            results[r_bracket] += (constant_normalization_factor / m_annular_shell_areas[r_bracket]);
                        }

                        if(length == 0)
                            within_radius_distribution += (constant_normalization_factor / M_PI);
                    }
                }
            }
        }
    }

    // Exponential difference
//    std::cout << "***** ANALYSIS SUMMARY *****" << std::endl;
//    std::cout << "# Reference points: " << reference_points.size() << std::endl;
//    std::cout << "# target points: " << target_points.size() << std::endl;
//    std::cout << "Analysis area: " << (width*height) << std::endl;
//    std::cout << "Average density: " << avg_density << std::endl;
//    std::cout << "Processed points: " << processed_points << std::endl;
//    std::cout << "Constant normalization factor: " << constant_normalization_factor << std::endl;
//    for(auto it(non_normalized_results.begin()); it != non_normalized_results.end(); it++)
//        std::cout << "R: " << it->first << " |  Count: " << it->second << std::endl;
//    std::cout << " Densities" << std::endl;
//    for(auto it(non_normalized_results.begin()); it != non_normalized_results.end(); it++)
//        std::cout << "R: " << it->first << " |  Density: " << (it->second/(m_annular_shell_areas[it->first]*reference_points.size())) << std::endl;
//    std::cout << " Annular shell areas " << std::endl;
//    for(auto it(m_annular_shell_areas.begin()); it != m_annular_shell_areas.end(); it++)
//        std::cout << "R: " << it->first << " |  Area: " << it->second << std::endl;

    return RadialDistribution(RadialDistributionProperties(reference_points_id, destination_points_id, reference_points.size(), target_points.size(), width*height, r_min, r_max, r_diff),
                           within_radius_distribution, results);
}

bool RadialDistributionAnalyzer::overflows_border(QPoint center, int r_bracket, int r_diff, int width, int height)
{
    int annular_shell_max_radius (r_bracket+r_diff);
    //TOP
    return (center.y() - annular_shell_max_radius < 0) ||
    // BOTTOM
            (center.y() + annular_shell_max_radius > height) ||
    // LEFT
            (center.x() - annular_shell_max_radius < 0) ||
    // RIGHT
            (center.x() + annular_shell_max_radius > width);
}

double RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint center, int radius, int width, int height)
{
    double max_circle_area(M_PI * pow(radius,2));

    double vertical_min_radius(std::min(radius,std::min(center.y(), height-center.y())));
    double horizontal_min_radius(std::min(radius,std::min(center.x(), width-center.x())));
    double vertical_angle(acos(vertical_min_radius/radius));
    double horizontal_angle(acos(horizontal_min_radius/radius));

    // Now add the remaining triangles to the area
    float vertical_triangle_side_length( sqrt(pow(radius,2) - pow(vertical_min_radius,2) ));
    float horizontal_triangle_side_length( sqrt(pow(radius,2) - pow(horizontal_min_radius,2) ));

    double area(max_circle_area);
    if(horizontal_min_radius != radius && vertical_min_radius != radius) // need to add triangles sperately and the residual square
    {
        double hidden_circle_angle(vertical_angle+horizontal_angle+(M_PI/2));
        area -= ((hidden_circle_angle/(2*M_PI)) * max_circle_area);
        area += (vertical_min_radius*vertical_triangle_side_length/2); // triangle 1
        area += (horizontal_min_radius*horizontal_triangle_side_length/2); // triangle 2
        area += (vertical_min_radius * horizontal_min_radius); // Residual square
    }
    else if(horizontal_min_radius != radius)
    {
        double hidden_circle_angle(2*horizontal_angle);
        area -= ((hidden_circle_angle/(2*M_PI)) * max_circle_area);
        area += (horizontal_min_radius * horizontal_triangle_side_length);
    }
    else if(vertical_min_radius != radius)
    {
        double hidden_circle_angle(2*vertical_angle);
        area -= ((hidden_circle_angle/(2*M_PI)) * max_circle_area);
        area += (vertical_min_radius * vertical_triangle_side_length);
    }

    return area;
}
