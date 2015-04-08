#ifndef RADIAL_DISTRIBUTION_REPRODUCER_H
#define RADIAL_DISTRIBUTION_REPRODUCER_H

#include "radial_distribution.h"
#include <string>
#include <QPoint>
#include "spatial_hashmap.h"
#include "point_map.h"
#include "point_properties.h"
#include "dice_roller.h"
#include "point_factory.h"

class AnalysisPoint;

class RadialDistributionReproducer
{
public:
    struct Settings{
        enum InitializationType{
            TwoPoints,
            MatchDensity
        };

        enum GenerationAlgorithm{
            BirthsAndDeaths,
            RandomMoves
        };

        int width, height, n_iterations;
        InitializationType initialization_type;
        GenerationAlgorithm generation_algo;

        Settings(int width, int height, int n_iterations, InitializationType init_type,
                 GenerationAlgorithm generation_algo) : width(width), height(height),
            n_iterations(n_iterations), initialization_type(init_type),
            generation_algo(generation_algo) {}
    };

    typedef std::map<std::pair<int,int>, RadialDistribution> PairCorrelations;
    typedef std::map<int,PointSizeProperties> CategorySizeProperties;
    typedef std::map<int,int> PriorityToCategoryMapper;

    RadialDistributionReproducer(std::vector<RadialDistribution> radial_distributions, std::vector<PointSizeProperties> point_size_properties,
                                 PriorityToCategoryMapper category_priorities, Settings reproduction_settings);

    void startPointGeneration();

    ~RadialDistributionReproducer();

    std::vector<AnalysisPoint*>& getGeneratedPoints();

private:
    void two_point_initialize();
    void matching_density_initialize();

    float calculateStrength(const AnalysisPoint* reference_point, const std::vector<AnalysisPoint*> & destination_points);
    float calculateStrength(const AnalysisPoint* reference_point);

    void generate_points_through_births_and_deaths();
    void generate_points_through_random_moves();

    void add_destination_point(AnalysisPoint * point);
    void remove_destination_point(AnalysisPoint * point, int destination_points_position);

    void generate_points();

    PairCorrelations m_pair_correlations;
    CategorySizeProperties m_category_size_properties;
    PriorityToCategoryMapper m_priority_to_category_ids;
    std::map<int,float> m_category_densities;

    std::vector<AnalysisPoint*> m_this_category_points;
    std::vector<AnalysisPoint*> m_all_generated_points;
    Settings m_settings;
    PointFactory m_point_factory;
    RadialDistributionSpatialHashmap m_generated_points_spatial_hashmap;

    int m_r_min, m_r_max, m_r_diff;
};

#endif //RADIAL_DISTRIBUTION_REPRODUCER_H
