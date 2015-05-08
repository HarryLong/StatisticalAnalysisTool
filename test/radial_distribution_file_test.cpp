#include "../radial_distribution.h"
#include "../analysis_configuration.h"

#include "test_data_factory.h"
#include "radial_distribution_file_test.h"

#include <QString>

void RadialDistributionFileTest::setUp()
{
    AnalysisConfiguration analysis_config (0, 100, 1, 500, 500) ;
    m_radial_distribution = TestDataFactory::generateSingleCategoryRadialDistribution(analysis_config, 1000, 1);
}

void RadialDistributionFileTest::tearDown()
{
    delete m_radial_distribution;
}

void RadialDistributionFileTest::testWriteRead()
{
    std::string test_file ("radial_distribution_test_file.tmp");
    m_radial_distribution->write(test_file);

    RadialDistribution loaded_radial_distribution ( test_file );

    // Test the header
    CPPUNIT_ASSERT(m_radial_distribution->m_header == loaded_radial_distribution.m_header);
    // Test the within radius distribution
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m_radial_distribution->m_within_radius_distribution, loaded_radial_distribution.m_within_radius_distribution, 0.001);
    // Test the past radius distribution
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m_radial_distribution->m_past_rmax_distribution, loaded_radial_distribution.m_past_rmax_distribution, 0.001);
    // Test the data
    CPPUNIT_ASSERT(m_radial_distribution->m_data.size() == loaded_radial_distribution.m_data.size());
    for(auto it(m_radial_distribution->m_data.begin()); it != m_radial_distribution->m_data.end(); it++)
    {
        auto loaded_it(loaded_radial_distribution.m_data.find(it->first));
        CPPUNIT_ASSERT(loaded_it != loaded_radial_distribution.m_data.end());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(it->second, loaded_it->second, 0.001);
    }
}
