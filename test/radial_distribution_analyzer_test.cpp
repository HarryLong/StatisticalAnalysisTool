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
    AnalysisConfiguration analysis_configuration(0,250,10,1000,1000);
    std::vector<int> priority_sorted_category_ids;
    priority_sorted_category_ids.push_back(1);

    RadialDistribution * radial_distribution ( TestDataFactory::generateSingleCategoryRadialDistribution(analysis_configuration,
                                                                                                         1500,
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
    // Generate analysis configuration
    AnalysisConfiguration analysis_config(0,125,10,500,500);

    std::vector<int> category_ids;
    category_ids.push_back(1);
    category_ids.push_back(2);
    analysis_config.setPrioritySortedCategoryIds(category_ids);


    // Create analyser
    RadialDistributionAnalyzer analyser (analysis_config);

    // Create points
    std::vector<AnalysisPoint*> category_1_points;
    category_1_points.push_back(new AnalysisPoint(1, QPoint(250,250), 10));

    // Add the category 2 points
    std::vector<AnalysisPoint*> category_2_points;
    for(int i(1); i < 10; i++)
    {
        category_2_points.push_back(new AnalysisPoint(2, QPoint(250-i,250), 1));
        category_2_points.push_back(new AnalysisPoint(2, QPoint(250,250-i), 1));

        category_2_points.push_back(new AnalysisPoint(2, QPoint(250+i,250), 1));
        category_2_points.push_back(new AnalysisPoint(2, QPoint(250,250+i), 1));
    }

    bool dependent;
    RadialDistribution radial_distribution(analyser.getRadialDistribution(category_2_points, category_1_points, 2, 1, dependent));

    CPPUNIT_ASSERT(dependent);

    CPPUNIT_ASSERT(radial_distribution.m_within_radius_distribution != 0);

    // All histogram values should equal to zero except for the range [0,10[
    for(auto it(radial_distribution.m_data.begin()); it != radial_distribution.m_data.end(); it++)
        CPPUNIT_ASSERT((it->first == 0 && it->second != 0) || it->second == 0);

    for(AnalysisPoint * p : category_1_points)
        delete p;

    for(AnalysisPoint * p : category_2_points)
        delete p;
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
