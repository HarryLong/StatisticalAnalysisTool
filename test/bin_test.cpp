#include "bin_test.h"
#include "../utils.h"
#include "../dice_roller.h"
#include <math.h>
#include <climits>

void BinTest::setUp()
{

}

void BinTest::tearDown()
{
    // Delete files
}

void BinTest::open_file_r()
{
    m_in_stream.open(BinTest::m_test_file_name, std::ios_base::binary);
    CPPUNIT_ASSERT(m_in_stream.is_open());
}

void BinTest::open_file_w()
{
    m_out_stream.open(BinTest::m_test_file_name, std::ios_base::binary | std::ios_base::trunc );
    CPPUNIT_ASSERT(m_out_stream.is_open());
}

int BinTest::get_file_size()
{
    m_in_stream.seekg(0, std::ios_base::end);
    int size(m_in_stream.tellg());
    m_in_stream.seekg(0, std::ios_base::beg);

    return size;
}

void BinTest::testInt()
{
    for(int size(1); size <= 8; size++)
    {
        DiceRoller dice_roller(0, std::min((double) INT_MAX, std::pow(2, size*8)-1));

        char * memblock = new char[size];
        for(int i(0); i < 1000; i++)
        {
            unsigned int random_value(dice_roller.generate());
            unsigned char * bin_random_value(Binutils::toBin(random_value,size));

            // Write
            open_file_w();
            m_out_stream.write((char*) bin_random_value, size);
            m_out_stream.close();

            // Read
            open_file_r();
            int input_file_size (get_file_size());
            CPPUNIT_ASSERT(input_file_size == size);
            m_in_stream.read(memblock, size);
            unsigned int read_value (Binutils::readInt32((unsigned char*) memblock,size));
            m_in_stream.close();

            CPPUNIT_ASSERT_MESSAGE("Read value: " + std::to_string(read_value) + " | Written value: " +
                                   std::to_string(random_value) + " (size: " + std::to_string(size) + ")", random_value == read_value);
        }
        delete [] memblock;
    }
}

// Floating point read write

void BinTest::testFloat()
{
    for(int size(4); size <= 8; size++)
    {
        DiceRoller dice_roller(0, std::pow(2, 2*8)-1);

        char * memblock = new char[size];
        for(int i(0); i < 1000; i++)
        {
            float random_value (dice_roller.generate());

            //fractional part
            {
                float fractional_part(dice_roller.generate());
                while(fractional_part > 1.0)
                    fractional_part /= 10;
                random_value += fractional_part;
            }

            unsigned char * bin_random_value(Binutils::toBin(random_value,size));

            // Write
            open_file_w();
            m_out_stream.write((char*) bin_random_value, size);
            m_out_stream.close();

            // Read
            open_file_r();
            int input_file_size (get_file_size());
            CPPUNIT_ASSERT(input_file_size == size);
            m_in_stream.read(memblock, size);
            float read_value (Binutils::readFloat32((unsigned char*) memblock,size));
            m_in_stream.close();

            CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Read value: " + std::to_string(read_value) + " | Written value: " +
                                                 std::to_string(random_value) + " (size: " + std::to_string(size) + ")",
                                                 read_value,
                                                 random_value,
                                                 .001);
        }
        delete [] memblock;
    }
}
