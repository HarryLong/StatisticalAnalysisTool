#include "analysis_configuration_file_test.h"
#include "../analyser/analysis_configuration.h"

void AnalysisiConfigurationFileTest::setUp()
{
    m_priority_sorted_category_ids.push_back(1);
    m_priority_sorted_category_ids.push_back(2);
    m_priority_sorted_category_ids.push_back(3);

    m_analysis_config = new AnalysisConfiguration(0,100,1,1000,1000,m_priority_sorted_category_ids);
}

void AnalysisiConfigurationFileTest::tearDown()
{
    delete m_analysis_config;
}

void AnalysisiConfigurationFileTest::testWriteRead()
{
    std::string test_file ("analysis_config_test_file.tmp");
    m_analysis_config->write(test_file);

    AnalysisConfiguration loaded_analysis_config( test_file );

    CPPUNIT_ASSERT(*m_analysis_config == loaded_analysis_config);
}
