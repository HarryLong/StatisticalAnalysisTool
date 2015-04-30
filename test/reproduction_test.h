#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <QString>

class ReproductionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( ReproductionTest );
    CPPUNIT_TEST( testRadialDistributionReproduction );
    CPPUNIT_TEST( testCategoricalReproduction );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testRadialDistributionReproduction();
    void testCategoricalReproduction();

private:
    void reset_test_directory();

    const QString m_production_test_dir = "./production_test_dir";
    const QString m_reproduction_test_dir = "./reproduction_test_dir";
};

CPPUNIT_TEST_SUITE_REGISTRATION( ReproductionTest );
