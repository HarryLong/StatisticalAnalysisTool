#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class RadialDistributionAnalyzerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( RadialDistributionAnalyzerTest );
    CPPUNIT_TEST( testBorderedCircleCalculations );
    CPPUNIT_TEST( testAnalysisConfigRespected );
    CPPUNIT_TEST( testRandomRadialDistribution );
    CPPUNIT_TEST( testDependentCategories );
    CPPUNIT_TEST( testRBracketCalculations );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testBorderedCircleCalculations();
    void testAnalysisConfigRespected();
    void testRandomRadialDistribution();
    void testDependentCategories();
    void testRBracketCalculations();

private:
};

CPPUNIT_TEST_SUITE_REGISTRATION( RadialDistributionAnalyzerTest );
