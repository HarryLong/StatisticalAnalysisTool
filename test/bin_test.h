#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

class BinTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BinTest );
    CPPUNIT_TEST( testInt );
    CPPUNIT_TEST( testFloat );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testInt();
    void testFloat();

private:
    void open_file_r();
    void open_file_w();
    int get_file_size();

    std::ifstream m_in_stream;
    std::ofstream m_out_stream;
    const std::string m_test_file_name = "binary_test_file";
};

CPPUNIT_TEST_SUITE_REGISTRATION( BinTest );
