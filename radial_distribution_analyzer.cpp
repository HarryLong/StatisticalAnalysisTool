#include "radial_distribution_analyzer.h"
#include <iostream>
#include <QLineF>
#include <math.h>
#include "utils.h"

RadialDistributionAnalyzer::RadialDistributionAnalyzer(int r_min, int r_max, int annular_shell_thickness) :
    r_min(r_min), r_max(r_max), r_diff(annular_shell_thickness)
{
    // Calculate annular shell areas
    for(int r (r_min); r <= r_max; r += annular_shell_thickness)
    {
        double area ( (M_PI * pow(r+annular_shell_thickness, 2)) - (M_PI * pow(r, 2)));
        m_annular_shell_areas.insert(std::pair<int,double>(r, area));
    }
}

RadialDistributionAnalyzer::~RadialDistributionAnalyzer()
{

}

RadialDistribution RadialDistributionAnalyzer::getRadialDistribution(std::vector<QPoint> reference_points, std::vector<QPoint> target_points,
                                                                     int width, int height, int reference_points_id, int destination_points_id)
{
    int total_area( width * height );

    // First calculate the average density
    float avg_density (((float)target_points.size()) / total_area);

    // initialize the histogram and calculate different annular shell areas
    RadialDistribution::Histogram results;
    for(int r (r_min); r <= r_max; r += r_diff)
    {
        results.insert(std::pair<int,int>(r,0.0));
    }

    double constant_normalization_factor((1.0f / reference_points.size()) / avg_density);

    // Set histogram values
    QLineF line;
    for(QPoint reference_point : reference_points)
    {
        line.setP1(reference_point);
        for(QPoint target_point : target_points)
        {
            if(reference_point != target_point)
            {
                line.setP2(target_point);
                float length(line.length());

                if(length > r_min && length <= r_max)
                {
                    int r_bracket(RadialDistributionUtils::getRBracket(line.length(), r_min, r_diff));

                    if(overflows_border(reference_point, r_bracket, width, height))
                    {
                        double small_circle_area(calculate_bordered_circle_area(reference_point, r_bracket, width, height));
                        double large_circle_area(calculate_bordered_circle_area(reference_point, r_bracket+r_diff, width, height));

                        double annular_shell_area (large_circle_area - small_circle_area);

                        results[r_bracket] += (constant_normalization_factor / annular_shell_area);
                    }
                    else
                    {
                        results[r_bracket] += (constant_normalization_factor / m_annular_shell_areas[r_bracket]);
                    }
                }
            }
        }
    }

    return RadialDistribution(RadialDistributionPorperties(reference_points_id, destination_points_id, reference_points.size(), target_points.size(), width*height, r_min, r_max, r_diff),
                           results);
}

bool RadialDistributionAnalyzer::overflows_border(QPoint center, int r_bracket, int width, int height)
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
