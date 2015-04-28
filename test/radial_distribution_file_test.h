#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class RadialDistribution;
class RadialDistributionFileTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( RadialDistributionFileTest );
    CPPUNIT_TEST( testWriteRead );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testWriteRead();

private:
    RadialDistribution * m_radial_distribution;
};

CPPUNIT_TEST_SUITE_REGISTRATION( RadialDistributionFileTest );
