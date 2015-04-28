#ifndef REPRODUCER_H
#define REPRODUCER_H

#include "radial_distribution.h"
#include <string>
#include <QPoint>
#include "point_spatial_hashmap.h"
#include "point_map.h"
#include "category_properties.h"
#include "dice_roller.h"
#include "point_factory.h"
#include "analysis_configuration.h"
#include "reproduction_configuration.h"

class AnalysisPoint;

class RadialDistributionReproducer
{
public:

    typedef std::map<std::pair<int,int>, RadialDistribution> PairCorrelations;
    typedef std::map<int,CategoryProperties> CategoryPropertiesContainer;

    static std::map<int,std::vector<AnalysisPoint*> > reproduce(ReproductionConfiguration reproduction_settings);

    ~RadialDistributionReproducer();


private:
    RadialDistributionReproducer(PairCorrelations pair_correlations, CategoryPropertiesContainer category_properties,
                                 ReproductionConfiguration reproduction_config, AnalysisConfiguration analysis_configuration);
    void startPointGeneration();
    void two_point_initialize();
    void matching_density_initialize();
    std::map<int,std::vector<AnalysisPoint*> >& getGeneratedPoints();

    float calculate_strength(const AnalysisPoint* reference_point, const std::vector<AnalysisPoint*> & destination_points);
    float calculate_strength(const AnalysisPoint* reference_point);

    void generate_points_through_births_and_deaths();
    void generate_points_through_random_moves();

    void add_destination_point(AnalysisPoint * point);
    void remove_destination_point(AnalysisPoint * point, int destination_points_position);

    void generate_points();

    PairCorrelations m_pair_correlations;
    CategoryPropertiesContainer m_category_properties;

    std::vector<AnalysisPoint*> m_active_category_points;
    std::map<int,std::vector<AnalysisPoint*> > m_all_generated_points;
    ReproductionConfiguration m_reproduction_configuration;
    PointFactory m_point_factory;
    PointSpatialHashmap m_spatial_point_storage;
    AnalysisConfiguration m_analysis_configuration;
};

#endif //REPRODUCER_H