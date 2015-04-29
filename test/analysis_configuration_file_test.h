#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class AnalysisConfiguration;
class AnalysisiConfigurationFileTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( AnalysisiConfigurationFileTest );
    CPPUNIT_TEST( testWriteRead );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testWriteRead();

private:
    AnalysisConfiguration * m_analysis_config;
    std::vector<int> m_priority_sorted_category_ids;
};

CPPUNIT_TEST_SUITE_REGISTRATION( AnalysisiConfigurationFileTest );
