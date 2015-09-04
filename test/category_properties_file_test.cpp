#include "category_properties_file_test.h"
#include "../category_properties.h"
#include "../analyser/category_analyzer.h"
#include "../reproducer/distribution_factory.h"

void CategoryPropertiesFileTest::setUp()
{
    // Generate random points
    DistributionFactory distribution_factory;
    std::vector<AnalysisPoint*> points(distribution_factory.generateRandomDistribution(1, 1000, 500, 500));

    CategoryAnalyzer category_analyzer(points, 1, 1);
    category_analyzer.calculateCategoryProperties(false);

    m_category_properties = new CategoryProperties(category_analyzer.getCategoryProperties());

    for(AnalysisPoint * p : points)
        delete p;
}

void CategoryPropertiesFileTest::tearDown()
{
    delete m_category_properties;
}

void CategoryPropertiesFileTest::testWriteRead()
{
    std::string test_file ("category_properties_test_file.tmp");
    m_category_properties->write(test_file);

    CategoryProperties loaded_category_properties ( test_file );

    // Test the header
    CPPUNIT_ASSERT(m_category_properties->m_header == loaded_category_properties.m_header);
    // Test the within radius distribution
    // Test the data
    CPPUNIT_ASSERT(m_category_properties->m_data.size() == loaded_category_properties.m_data.size());
    for(auto it(m_category_properties->m_data.begin()); it != m_category_properties->m_data.end(); it++)
    {
        auto loaded_it(loaded_category_properties.m_data.find(it->first));
        CPPUNIT_ASSERT(loaded_it != loaded_category_properties.m_data.end());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(it->second, loaded_it->second, 0.001);
    }
}
