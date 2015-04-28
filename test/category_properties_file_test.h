#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class CategoryProperties;
class CategoryPropertiesFileTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( CategoryPropertiesFileTest );
    CPPUNIT_TEST( testWriteRead );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testWriteRead();

private:
    CategoryProperties * m_category_properties;
};

CPPUNIT_TEST_SUITE_REGISTRATION( CategoryPropertiesFileTest );
