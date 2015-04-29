#include "radial_distribution_analyzer_test.h"
#include "test_data_factory.h"

#include "../radial_distribution_analyzer.h"
#include "../analysis_configuration.h"
#include "../utils.h"

void RadialDistributionAnalyzerTest::setUp()
{

}

void RadialDistributionAnalyzerTest::tearDown()
{

}

void RadialDistributionAnalyzerTest::testBorderedCircleCalculations()
{
    //    std::cout << "Testing bordered circles area calculations..." << std::endl;
    //    RadialDistributionAnalyzer analyzer(1000,1000, 1, 100, 1);
    double delta(.1);
    // Cut off from the top
    {
        double expected(1010.96);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(50,10),20,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut off from the bottom
    {
        double expected(1010.96);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(50,90),20,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut off from the left
    {
        double expected(1010.96);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(10,50),20,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut off from the Right
    {
        double expected(1010.96);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(90,50),20,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut from top left
    {
        double expected(1753.1);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(10,20),30,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut from top right
    {
        double expected(1753.19);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(90,20),30,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut from bttom left
    {
        double expected(1753.19);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(10,80),30,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Cut from bottom right
    {
        double expected(1753.19);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(90,80),30,100,100));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }

    // Another random, more complicated scenario
    {
        double expected(111149.4);
        double calculated (RadialDistributionAnalyzer::calculate_bordered_circle_area(QPoint(902,790),212,1000,1000));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, calculated, delta);
    }
}

void RadialDistributionAnalyzerTest::testRandomRadialDistribution()
{
    AnalysisConfiguration analysis_configuration(0,250,5,1000,1000);
    std::vector<int> priority_sorted_category_ids;
    priority_sorted_category_ids.push_back(1);

    RadialDistribution * radial_distribution ( TestDataFactory::generateSingleCategoryRadialDistribution(analysis_configuration,
                                                                                                         500,
                                                                                                         1) );

    // The values must be around 1
    double delta(0.1);
    for(auto it(radial_distribution->m_data.begin()); it != radial_distribution->m_data.end(); it++)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, it->second, delta);
    }
}

void RadialDistributionAnalyzerTest::testAnalysisConfigRespected()
{
    AnalysisConfiguration analysis_configuration(0,125,5,500,500);
    std::vector<int> priority_sorted_category_ids;
    priority_sorted_category_ids.push_back(1);

    RadialDistribution * radial_distribution ( TestDataFactory::generateSingleCategoryRadialDistribution(analysis_configuration,
                                                                                                         500,
                                                                                                         1) );

    // Ensure correct number of values
    CPPUNIT_ASSERT(radial_distribution->m_data.size() == (125/5));

    // Ensure all values are present and are seperated by r diff = 5
    for(int i = 0; i < 125; i+= 5)
    {
        CPPUNIT_ASSERT(radial_distribution->m_data.find(i) != radial_distribution->m_data.end());
    }
}

void RadialDistributionAnalyzerTest::testDependentCategories()
{

}

void RadialDistributionAnalyzerTest::testRBracketCalculations()
{
    {
        int expected(0);
        int calculated( RadialDistributionUtils::getRBracket(6, 0, 7) );
        CPPUNIT_ASSERT(expected == calculated);
    }
    {
        int expected(6);
        int calculated( RadialDistributionUtils::getRBracket(6, 0, 6) );
        CPPUNIT_ASSERT(expected == calculated);
    }
}
