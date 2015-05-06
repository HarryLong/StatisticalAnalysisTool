#include "reproduction_test.h"
#include "test_data_factory.h"
#include "../reproducer.h"
#include "../analyzer.h"
#include "../reproduction_configuration.h"
#include "../distribution_factory.h"
#include "../utils.h"
#include <QDir>

void ReproductionTest::setUp()
{
    reset_test_directory();

    // Generate production to reproduce
    AnalysisConfiguration analysis_config(0, 100, 5, 1000, 1000);
    std::vector<int> priority_sorted_category_ids;
    priority_sorted_category_ids.push_back(1);
    analysis_config.setPrioritySortedCategoryIds(priority_sorted_category_ids);
    {
        DistributionFactory factory;
        std::map<int, std::vector<AnalysisPoint*> >analysis_points;
        analysis_points[1] = (factory.generateRandomDistribution(1, 1000, 1000, 1000, 1, 10) );
        Analyzer::analyze(m_production_test_dir, analysis_points, analysis_config);
        for(auto category_it(analysis_points.begin()); category_it != analysis_points.end(); category_it++)
            for(auto points_it(category_it->second.begin()); points_it != category_it->second.end(); points_it++)
                delete *points_it;
    }

    // Reproduce
    ReproductionConfiguration reproduction_config(m_production_test_dir, 1000, 1000, 10000);
    std::map<int,std::vector<AnalysisPoint*> > reproduced_points (RadialDistributionReproducer::reproduce(reproduction_config));
    Analyzer::analyze(m_reproduction_test_dir, reproduced_points, analysis_config);
}

void ReproductionTest::tearDown()
{
//    reset_test_directory();
}

void ReproductionTest::reset_test_directory()
{
    {
        QDir directory(m_production_test_dir);
        CPPUNIT_ASSERT(directory.removeRecursively());
        CPPUNIT_ASSERT(directory.mkpath("."));
    }
    {
        QDir directory(m_reproduction_test_dir);
        CPPUNIT_ASSERT(directory.removeRecursively());
        CPPUNIT_ASSERT(directory.mkpath("."));
    }
}

void ReproductionTest::testRadialDistributionReproduction()
{
    std::vector<QString> produced_radial_distribution_files(FileUtils::get_radial_distribution_files(m_production_test_dir));
    std::vector<QString> reproduced_radial_distribution_files(FileUtils::get_radial_distribution_files(m_reproduction_test_dir));

    CPPUNIT_ASSERT(produced_radial_distribution_files.size() == 1);
    CPPUNIT_ASSERT(reproduced_radial_distribution_files.size() == 1);

    RadialDistribution produced_radial_distribution = RadialDistribution(produced_radial_distribution_files.at(0).toStdString());
    RadialDistribution reproduced_radial_distribution = RadialDistribution(reproduced_radial_distribution_files.at(0).toStdString());

    CPPUNIT_ASSERT(produced_radial_distribution.m_header == reproduced_radial_distribution.m_header);
    // Test the within radius distribution
    CPPUNIT_ASSERT_DOUBLES_EQUAL(produced_radial_distribution.m_within_radius_distribution,
                                 reproduced_radial_distribution.m_within_radius_distribution, 0.1);
    // Test the data
    CPPUNIT_ASSERT(produced_radial_distribution.m_data.size() == reproduced_radial_distribution.m_data.size());
    for(auto it(produced_radial_distribution.m_data.begin()); it != produced_radial_distribution.m_data.end(); it++)
    {
        auto reproduced_it(reproduced_radial_distribution.m_data.find(it->first));
        CPPUNIT_ASSERT(reproduced_it != reproduced_radial_distribution.m_data.end());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(it->second, reproduced_it->second, 0.2); // Allow 10%
    }
}
