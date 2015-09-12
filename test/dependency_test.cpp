#include "dependency_test.h"
#include "../analyser/dependency_analyzer.h"
#include "../analysis_point.h"
#include "../reproducer/distribution_factory.h"

#include <QPoint>

void DependencyTest::setUp()
{
    setup_dependent_points();
    setup_independent_points();
}

void DependencyTest::tearDown()
{

}

void DependencyTest::setup_dependent_points()
{
    std::vector<AnalysisPoint> category_1_points;
    category_1_points.push_back(AnalysisPoint(1, QPoint(250,250), 10));
    category_1_points.push_back(AnalysisPoint(1, QPoint(200,200), 5));
    category_1_points.push_back(AnalysisPoint(1, QPoint(80,80), 5));
    category_1_points.push_back(AnalysisPoint(1, QPoint(20,20), 10));

    m_dependent_points.emplace(1, category_1_points);

    // Add the category 2 points
    std::vector<AnalysisPoint> category_2_points;
    for(int i(1); i < 10; i++)
    {
        category_2_points.push_back(AnalysisPoint(2, QPoint(250-i,250), 1));
        category_2_points.push_back(AnalysisPoint(2, QPoint(250,250-i), 1));

        category_2_points.push_back(AnalysisPoint(2, QPoint(250+i,250), 1));
        category_2_points.push_back(AnalysisPoint(2, QPoint(250,250+i), 1));
    }
    m_dependent_points.emplace(2, category_2_points);
}

void DependencyTest::setup_independent_points()
{
    DistributionFactory factory;
    std::vector<AnalysisPoint> c1_points(factory.generateRandomDistribution(1, 500, 240, 500, 1, 10));
    std::vector<AnalysisPoint> c2_points(factory.generateRandomDistribution(2, 500, 240, 500, 1, 10));

    //shift all c2 points by 250
    for(AnalysisPoint & p : c2_points)
        p.setCenter(QPoint(p.getCenter().x() + 250, p.getCenter().y()));

    m_independent_points.emplace(1, c1_points);
    m_independent_points.emplace(2, c2_points);
}

void DependencyTest::testInDependent()
{
    DependencyAnalyzer::Dependencies dependencies(DependencyAnalyzer::getDependencies(m_independent_points, 500, 500));

    CPPUNIT_ASSERT(dependencies.size() == 0);
}

void DependencyTest::testDependent()
{
    DependencyAnalyzer::Dependencies dependencies(DependencyAnalyzer::getDependencies(m_dependent_points, 500, 500));

    CPPUNIT_ASSERT(dependencies.size() == 1);

    CPPUNIT_ASSERT(dependencies.find(2) != dependencies.end());

    CPPUNIT_ASSERT(dependencies[2].size() == 1);
    CPPUNIT_ASSERT(dependencies[2].find(1) != dependencies[2].end());
}
