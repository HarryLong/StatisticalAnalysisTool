#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include <vector>

class AnalysisPoint;
class DependencyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( DependencyTest );
    CPPUNIT_TEST( testDependent );
    CPPUNIT_TEST( testInDependent );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testDependent();
    void testInDependent();

private:
    void setup_dependent_points();
    void setup_independent_points();
    std::map<int, std::vector<AnalysisPoint> > m_dependent_points;
    std::map<int, std::vector<AnalysisPoint> > m_independent_points;
};

CPPUNIT_TEST_SUITE_REGISTRATION( DependencyTest );
