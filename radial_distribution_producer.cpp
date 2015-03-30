#include "radial_distribution_producer.h"
#include <math.h>
#include "utils.h"
#include <QLineF>
#include <iostream>

RadialDistributionProducer::RadialDistributionProducer(std::string radial_distribution_file):
    m_radial_distribution(radial_distribution_file)
{

}

RadialDistributionProducer::RadialDistributionProducer(RadialDistribution distribution) :
    m_radial_distribution(distribution)
{

}

RadialDistributionProducer::~RadialDistributionProducer()
{

}

std::vector<QPoint> RadialDistributionProducer::generatePoints(int width, int height, int iterations, bool verbose)
{
    float area(width*height);

    // First generate two points at random
    float initial_strength(.0f);
    std::vector<QPoint> generated_points (initialize(width, height, initial_strength));

    // Calculate the initial strength
    double strength(initial_strength);

    float birth_strength(.0f);
    float birth_point_strength(.0f);
    float birth_accepetance_ratio(.0f);
    int birth_points_within_r_max(0);

    float death_strength(.0f);
    float death_point_strength(.0f);
    float death_acceptance_ratio(.0f);
    int death_points_within_r_max(0);

    int births_accepted(0);
    int deaths_accepted(0);

    if(verbose)
        std::cout << "Initial strength: " << strength << std::endl;

    DiceRoller dice_roller(0,1000);

    for(int i(0); i < iterations; i++)
    {
        if(ProbabilisticUtils::returnTrueWithProbability(0.5, dice_roller))
        // Birth
        {
            QPoint random_point(get_random_point(width, height));

            birth_point_strength = (calculateStrength(random_point, generated_points, birth_points_within_r_max));
            birth_point_strength *= birth_point_strength;

            birth_strength = (strength * birth_point_strength);
            birth_accepetance_ratio = birth_point_strength * (area/(generated_points.size()+1));

            if(birth_points_within_r_max == 0 || ProbabilisticUtils::returnTrueWithProbability(birth_accepetance_ratio, dice_roller)) // If the point added is further than r_max for all points, just add it
            {
                births_accepted++;
                generated_points.push_back(random_point);
                birth = true;
                if(birth_points_within_r_max > 0)
                    strength = birth_strength;
            }
        }
        else // Death
        {
            if(generated_points.size() > 2)
            {
                // Get a random point from the generated points
                int selected_point_index (rand()%generated_points.size());
                QPoint selected_point( generated_points.at(selected_point_index) );

                // Remove it from the list on generated points
                generated_points.erase(generated_points.begin()+selected_point_index);

                death_point_strength = calculateStrength(selected_point, generated_points, death_points_within_r_max);
                death_point_strength *= death_point_strength;

                death_strength = (strength / death_point_strength); // Death point strength will never be zero as it would otherwise not have been placed
                death_acceptance_ratio = (1.0f/death_point_strength) * (generated_points.size()/area);

                if(death_points_within_r_max == 0 || !ProbabilisticUtils::returnTrueWithProbability(death_acceptance_ratio, dice_roller)) // If the point removed was further than r_max for all points, leeave it
                {
                    generated_points.push_back(selected_point); // push back point
                }
                else
                {
                    deaths_accepted++;
                    death = true;
                    strength = death_strength;
                }
            }
        }
        if(verbose)
        {
            std::cout << "Iteration " << i << std::endl
                         << "- Birth --> Points within r_max: " << birth_points_within_r_max << " | Point strength: " << birth_point_strength << " | Acceptance ratio: " << birth_accepetance_ratio << " | Aggregate strength if accepted: " << birth_strength << " | Accepted: " << (birth ? "Yes!" : "No!") << std::endl
                         << "- Death --> Points within r_max: " << death_points_within_r_max << " | Point strength: " << death_point_strength << " | Acceptance ratio: " << death_acceptance_ratio << " | Aggregate strength if accepted: " << death_strength << " | Accepted: " << (death ? "Yes!" : "No!") << std::endl
                         << "Strength: " << strength << std::endl;
            std::cout << "*******************************************************************************************" << std::endl;
         }
    }
    std::cout << "Simulation over! " << std::endl
              << "\t Start strength: " << initial_strength << " | End strength: " << strength << std::endl
              << "\t Births: " << births_accepted << std::endl
              << "\t Deaths: " << deaths_accepted << std::endl
              << "\t Total points: " << generated_points.size() << std::endl;

    return generated_points;
}

std::vector<QPoint> RadialDistributionProducer::initialize(int width, int height,float & strength)
{
    QPoint p1(width/2.0f, height/2.0f), p2(width/2.0f, height/2.0f);

    bool point_found(false);
    int p2_x(p1.x());
    while(!point_found && (p2_x += m_radial_distribution.m_properties.r_diff) < width)
    {
        if(m_radial_distribution.m_data[RadialDistributionUtils::getRBracket(p2_x-p1.x(), m_radial_distribution.m_properties.r_min, m_radial_distribution.m_properties.r_diff)] > 0)
        {
            p2.setX(p2_x);
            point_found = true;
        }
    }

    if(!point_found)
    {
        std::cerr << "Unable to initialize points with non-zero strength!" << std::endl;
        exit(1);
    }


    std::vector<QPoint> ret;
    int within_r_max;
    ret.push_back(p1);
    strength = pow(calculateStrength(p2, ret, within_r_max),2);

    if(within_r_max == 0)
    {
        std::cerr << "Unable to initialize points with non-zero strength!" << std::endl;
        exit(1);
    }

    ret.push_back(p2);

    return ret;
}

float RadialDistributionProducer::calculateStrength(QPoint reference_point, std::vector<QPoint> destination_points, int & points_within_r_max)
{
    points_within_r_max = 0;
    float strength(1.0f);

    QLineF line;
    line.setP1(reference_point);
    for(QPoint destination_point : destination_points)
    {
        line.setP2(destination_point);
        int r_bracket ( RadialDistributionUtils::getRBracket(line.length(), m_radial_distribution.m_properties.r_min, m_radial_distribution.m_properties.r_diff) );
        if(r_bracket <= m_radial_distribution.m_properties.r_max)
        {
            points_within_r_max++;
            strength *= m_radial_distribution.m_data.find(r_bracket)->second;
        }
    }

    return strength;
}

QPoint RadialDistributionProducer::get_random_point(int max_width, int max_height)
{
   return QPoint(rand() % max_width, rand() % max_height);
}
