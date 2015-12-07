#ifndef REPRODUCER_H
#define REPRODUCER_H

#include <string>
#include <QPoint>
#include <QLineF>

#include "../radial_distribution.h"
#include "../point_spatial_hashmap.h"
#include "../category_properties.h"
#include "../analyser/analysis_configuration.h"
#include "point_factory.h"
#include "reproduction_configuration.h"

class AnalysisPoint;

class StrengthCalculationTracker{
public:
    StrengthCalculationTracker(int count);
    ~StrengthCalculationTracker();

    void complete(float strength) const;

    bool finished() const;

    float strength() const;

private:
    const int _n_total;
    mutable int _n_complete;
    mutable float _strength;
};

//-----------------------------------------------------------

class StrengthCalculationSpec{
public:
    StrengthCalculationSpec(const StrengthCalculationTracker & tracker, QLineF line, int r_min, int r_diff, int r_max,
                            const RadialDistribution & radial_distribution, const AnalysisPoint & destination_point);
    ~StrengthCalculationSpec();

    void calculate();


private:
    const StrengthCalculationTracker & tracker;
    QLineF line;
    int r_min;
    int r_diff;
    int r_max;
    const RadialDistribution & radial_distribution;
    const AnalysisPoint & destination_point;
};

//-----------------------------------------------------------

class RadialDistributionReproducer
{
public:

    typedef std::map<std::pair<int,int>, RadialDistribution> PairCorrelations;
    typedef std::map<int,CategoryProperties> CategoryPropertiesContainer;

    static std::map<int,std::vector<AnalysisPoint> > reproduce(ReproductionConfiguration reproduction_settings);

    ~RadialDistributionReproducer();


private:
    RadialDistributionReproducer(PairCorrelations pair_correlations, CategoryPropertiesContainer category_properties,
                                 ReproductionConfiguration reproduction_config, AnalysisConfiguration analysis_configuration);
    void startPointGeneration();
    void two_point_initialize();
    int matching_density_initialize();
    std::map<int,std::vector<AnalysisPoint> >& getGeneratedPoints();

    void accelerated_point_validity_check(const AnalysisPoint & reference_point, bool & valid, bool & strength_calculation_necessary);
    void accelerated_point_validity_check(const AnalysisPoint & reference_point, int queried_category, bool & valid, bool & strength_calculation_necessary);
    float calculate_strength(const AnalysisPoint & reference_point, const std::vector<AnalysisPoint> & destination_points);
    float calculate_strength(const AnalysisPoint & reference_point);

    void generate_points_through_births_and_deaths();
    void generate_points_through_random_moves(int n_moves);

    void add_destination_point(const AnalysisPoint & point);
    void remove_destination_point(const AnalysisPoint & point, int destination_points_position);

    bool requires_optimization(int category_id);

    void generate_points();

    const RadialDistribution & get_radial_distribution(int reference_category, int target_category);

    PairCorrelations m_pair_correlations;
    CategoryPropertiesContainer m_category_properties;

    std::vector<AnalysisPoint> m_active_category_points;
    std::map<int,std::vector<AnalysisPoint> > m_all_generated_points;
    ReproductionConfiguration m_reproduction_configuration;
    PointFactory m_point_factory;
    PointSpatialHashmap m_spatial_point_storage;
    AnalysisConfiguration m_analysis_configuration;
};

#endif //REPRODUCER_H
