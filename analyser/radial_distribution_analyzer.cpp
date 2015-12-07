#include "radial_distribution_analyzer.h"
#include <iostream>
#include <QLineF>
#include <math.h>
#include "../utils/utils.h"
#include "../point_spatial_hashmap.h"

RadialDistributionAnalyzer::RadialDistributionAnalyzer(AnalysisConfiguration analysis_configuration, const std::vector<AnalysisPoint> reference_points,
                           const std::vector<AnalysisPoint> target_points, int reference_points_id, int destination_points_id, CompletionListener * m_completion_listener) :
    m_analysis_configuration(analysis_configuration), m_reference_points(reference_points), m_target_points(target_points),
    m_reference_points_id(reference_points_id), m_target_points_id(destination_points_id), m_completion_listener(m_completion_listener)
{
    if((analysis_configuration.r_max-analysis_configuration.r_min) % analysis_configuration.r_diff != 0)
    {
        std::cout << "R max increased from " << analysis_configuration.r_max << " to " << std::flush;
        analysis_configuration.r_max = analysis_configuration.r_min +
                ((((analysis_configuration.r_max-analysis_configuration.r_min)/analysis_configuration.r_diff) + 1) * analysis_configuration.r_diff);
        std::cout << analysis_configuration.r_max << std::endl;
    }
}

RadialDistributionAnalyzer::~RadialDistributionAnalyzer()
{

}

RadialDistribution RadialDistributionAnalyzer::getRadialDistribution()
{
    return m_radial_distribution;
}

void RadialDistributionAnalyzer::calculateRadialDistribution(bool asynchronous)
{
    if(asynchronous)
        std::thread(&RadialDistributionAnalyzer::calculate_radial_distribution, this).detach();
    else
        calculate_radial_distribution();
}

void RadialDistributionAnalyzer::calculate_radial_distribution()
{
    // Optimization: build a spatial hashmap
    PointSpatialHashmap spatial_point_storage(m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height);

    for(const AnalysisPoint & p : m_target_points)
    {
        spatial_point_storage.getCell(p.getCenter(), PointSpatialHashmap::_WORLD).points.insert(p);
    }

    int total_area( m_analysis_configuration.analysis_window_width * m_analysis_configuration.analysis_window_height );

    // initialize the histogram and calculate different annular shell areas
    RadialDistribution::Histogram results;

    for(int r (m_analysis_configuration.r_min); r < m_analysis_configuration.r_max; r += m_analysis_configuration.r_diff)
        results.insert(std::pair<int,int>(r,0.0));

    float within_radius_distribution(.0f);
    float past_rmax_distribution(.0f);

    double constant_normalization_factor(((float)total_area) / (m_reference_points.size()*m_target_points.size()));
    // Set histogram values
    QLineF line;

    int n_pairs_processed(0);
    int reference_points_processed(0);
    for(const AnalysisPoint & reference_point : m_reference_points)
    {
        if(reference_points_processed++ % 100 == 0)
            std::cout << "Points processed: " << reference_points_processed << " / " << m_reference_points.size() << std::endl;

        std::vector<AnalysisPoint> possible_reachable_points(spatial_point_storage.getPossibleReachablePoints(reference_point, m_analysis_configuration.r_max));
        int processed_points(0);

        std::map<int,double> annular_shell_areas; // Caching
        // Within radius area
        float within_radius_area;
        if(RadialDistributionAnalyzer::overflows_border(reference_point.getCenter(), reference_point.getRadius(), m_analysis_configuration.analysis_window_width,
                                                            m_analysis_configuration.analysis_window_height))
        {
            within_radius_area = RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point.getCenter(), reference_point.getRadius(),
                                                                m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height);
        }
        else
            within_radius_area = GeometricUtils::getCircleArea(reference_point.getRadius());

        if(possible_reachable_points.size() > 0)
        {
            n_pairs_processed += possible_reachable_points.size();
            line.setP1(reference_point.getCenter());

            for(AnalysisPoint & target_point : possible_reachable_points)
            {
                line.setP2(target_point.getCenter());

                float distance(line.length()-reference_point.getRadius()); // Distance from the reference points circumference [remove the radius]

                if((m_analysis_configuration.r_min == 0 || distance >= m_analysis_configuration.r_min) && distance < m_analysis_configuration.r_max)
                {
                    processed_points++;
                    if(distance < .0f) // Within radius
                    {
                        within_radius_distribution += (constant_normalization_factor / within_radius_area);
                    }
                    else // Outside radius
                    {
                        int r_bracket(RadialDistributionUtils::getRBracket(distance, m_analysis_configuration.r_min, m_analysis_configuration.r_diff));
                        if(annular_shell_areas.find(r_bracket) == annular_shell_areas.end())
                        {
                            // Small circle
                            int small_circle_radius( reference_point.getRadius() + r_bracket );
                            double small_circle_area;
                            if(RadialDistributionAnalyzer::overflows_border(reference_point.getCenter(), small_circle_radius,
                                                                            m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height))
                            {
                                small_circle_area =
                                        RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point.getCenter(), small_circle_radius,
                                                            m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height);
                            }
                            else
                            {
                                small_circle_area = GeometricUtils::getCircleArea(small_circle_radius);
                            }

                            // Large circle
                            int large_circle_radius (small_circle_radius + m_analysis_configuration.r_diff);
                            double large_circle_area;
                            if(RadialDistributionAnalyzer::overflows_border(reference_point.getCenter(), large_circle_radius,
                                                                            m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height))
                            {
                                large_circle_area =
                                        RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point.getCenter(), large_circle_radius,
                                                            m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height);
                            }
                            else
                            {
                                large_circle_area = GeometricUtils::getCircleArea(large_circle_radius);
                            }
                            double area(large_circle_area - small_circle_area);

                            annular_shell_areas.insert(std::pair<int,double>(r_bracket, area));
                        }
                        results[r_bracket] += (constant_normalization_factor / annular_shell_areas[r_bracket]);
                    }
                }
            }
            // Remaining points
            // Past radius area
            float past_radius_area;
            if(RadialDistributionAnalyzer::overflows_border(reference_point.getCenter(), m_analysis_configuration.r_max+ reference_point.getRadius(), m_analysis_configuration.analysis_window_width,
                                                                m_analysis_configuration.analysis_window_height))
            {
                double large_circle_area = RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point.getCenter(), m_analysis_configuration.r_max+ reference_point.getRadius(),
                                                                    m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height);
                past_radius_area = (m_analysis_configuration.analysis_window_height*m_analysis_configuration.analysis_window_width) - large_circle_area;
            }
            else
            {
                past_radius_area = (m_analysis_configuration.analysis_window_height*m_analysis_configuration.analysis_window_width) -
                        GeometricUtils::getCircleArea(m_analysis_configuration.r_max+ reference_point.getRadius());
            }
            past_rmax_distribution += (m_target_points.size()-processed_points)*(constant_normalization_factor / past_radius_area);
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

    bool requires_optimization(within_radius_distribution > 0 && std::abs(within_radius_distribution-1) > .2);
    for(auto it(results.begin()); it != results.end() && !requires_optimization; it++)
    {
        if(std::abs(it->second-1) > .2 )
            requires_optimization = true;
    }

    m_radial_distribution = RadialDistribution(RadialDistributionHeader(m_reference_points_id, m_target_points_id, requires_optimization), within_radius_distribution, past_rmax_distribution, results);
    if(m_completion_listener)
        m_completion_listener->complete(m_radial_distribution);
}
//RadialDistribution RadialDistributionAnalyzer::getRadialDistribution(std::vector<AnalysisPoint*> & reference_points, std::vector<AnalysisPoint*> & target_points,
//                                                                     int reference_points_id, int destination_points_id)
//{
//        // Optimization: build a spatial hashmap
//        PointSpatialHashmap spatial_point_storage(m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height);

//        for(AnalysisPoint* p : target_points)
//            spatial_point_storage.getCell(p->getCenter(), true)->points.push_back(p);

//        int total_area( m_analysis_configuration.analysis_window_width * m_analysis_configuration.analysis_window_height );

//        // First calculate the average density
//        float avg_density (((float)target_points.size()) / total_area);

//        // initialize the histogram and calculate different annular shell areas
//        RadialDistribution::Histogram results;

//        for(int r (m_analysis_configuration.r_min); r < m_analysis_configuration.r_max; r += m_analysis_configuration.r_diff)
//            results.insert(std::pair<int,int>(r,0.0));

//        float within_radius_distribution(.0f);

//        double constant_normalization_factor(((float)m_analysis_configuration.analysis_window_width*m_analysis_configuration.analysis_window_height) / (reference_points.size()*target_points.size()));
//        // Set histogram values
//        QLineF line;
//        for(AnalysisPoint * reference_point : reference_points)
//        {
//            line.setP1(reference_point->getCenter());

//            int normalization_length(reference_point->getRadius()-1); // Perform analysis for each point normalized to a length of 1

//            std::vector<PointSpatialHashMapCell*> cells_within_r_max(
//                        spatial_point_storage.getCells(reference_point, m_analysis_configuration.r_max+normalization_length)); // Add cell width so that it returns all cells within reach

//            for(PointSpatialHashMapCell * cell : cells_within_r_max)
//            {
//                for(AnalysisPoint* target_point : cell->points)
//                {
//                    if(reference_point != target_point)
//                    {
//                        line.setP2(target_point->getCenter());
//                        float length(std::max(0.0,line.length()-normalization_length));
//                        if(length >= m_analysis_configuration.r_min && length < m_analysis_configuration.r_max)
//                        {
//                            int r_bracket(RadialDistributionUtils::getRBracket(length, m_analysis_configuration.r_min, m_analysis_configuration.r_diff));

//                            if(RadialDistributionAnalyzer::overflows_border(reference_point->getCenter(), r_bracket, m_analysis_configuration.r_diff, m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height))
//                            {
//                                double small_circle_area(RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point->getCenter(), r_bracket, m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height));
//                                double large_circle_area(RadialDistributionAnalyzer::calculate_bordered_circle_area(reference_point->getCenter(), r_bracket+m_analysis_configuration.r_diff, m_analysis_configuration.analysis_window_width, m_analysis_configuration.analysis_window_height));

//                                double annular_shell_area (large_circle_area - small_circle_area);

//                                results[r_bracket] += (constant_normalization_factor / annular_shell_area);
//                            }
//                            else
//                            {
//                                results[r_bracket] += (constant_normalization_factor / m_annular_shell_areas[r_bracket]);
//                            }

//                            if(length == 0)
//                                within_radius_distribution += (constant_normalization_factor / M_PI);
//                        }
//                    }
//                }
//            }
//        }

//        // Exponential difference
//    //    std::cout << "***** ANALYSIS SUMMARY *****" << std::endl;
//    //    std::cout << "# Reference points: " << reference_points.size() << std::endl;
//    //    std::cout << "# target points: " << target_points.size() << std::endl;
//    //    std::cout << "Analysis area: " << (width*height) << std::endl;
//    //    std::cout << "Average density: " << avg_density << std::endl;
//    //    std::cout << "Processed points: " << processed_points << std::endl;
//    //    std::cout << "Constant normalization factor: " << constant_normalization_factor << std::endl;
//    //    for(auto it(non_normalized_results.begin()); it != non_normalized_results.end(); it++)
//    //        std::cout << "R: " << it->first << " |  Count: " << it->second << std::endl;
//    //    std::cout << " Densities" << std::endl;
//    //    for(auto it(non_normalized_results.begin()); it != non_normalized_results.end(); it++)
//    //        std::cout << "R: " << it->first << " |  Density: " << (it->second/(m_annular_shell_areas[it->first]*reference_points.size())) << std::endl;
//    //    std::cout << " Annular shell areas " << std::endl;
//    //    for(auto it(m_annular_shell_areas.begin()); it != m_annular_shell_areas.end(); it++)
//    //        std::cout << "R: " << it->first << " |  Area: " << it->second << std::endl;

//        return RadialDistribution(RadialDistributionProperties(reference_points_id, destination_points_id, reference_points.size(), target_points.size(), m_analysis_configuration.analysis_window_width*m_analysis_configuration.analysis_window_height, m_analysis_configuration.r_min, m_analysis_configuration.r_max, m_analysis_configuration.r_diff),
//                               within_radius_distribution, results);
//}

bool RadialDistributionAnalyzer::overflows_border(QPoint center, int radius, int window_width, int window_height)
{
    //TOP
    return (center.y() - radius < 0) ||
    // BOTTOM
            (center.y() + radius > window_height) ||
    // LEFT
            (center.x() - radius < 0) ||
    // RIGHT
            (center.x() + radius > window_width);
}

double RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint center, int radius, int width, int height)
{
    double max_circle_area(GeometricUtils::getCircleArea(radius));

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
