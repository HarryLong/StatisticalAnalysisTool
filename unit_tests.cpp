#include "unit_tests.h"
#include "radial_distribution_analyzer.h"
#include <cassert>
#include <iostream>
#include "utils.h"
#include <iostream>
#include <fstream>

TestClass::TestClass()
{

}

void TestClass::runTests()
{
    test1();
    test2();
    test3();
}

void TestClass::test1()
{
    std::cout << "Testing bordered circles area calculations..." << std::endl;
    RadialDistributionAnalyzer analyzer(1, 100, 1);

    // Cut off from the top
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from top test. Expected Area: 1010.96 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(50,10),20,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1010.9 && area < 1011);
        std::cout << "Passed!" << std::endl;
    }

    // Cut off from the bottom
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from botom test. Expected Area: 1010.96 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(50,90),20,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1010.9 && area < 1011);
        std::cout << "Passed!" << std::endl;
    }

    // Cut off from the left
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from left test. Expected Area: 1010.96 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(10,50),20,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1010.9 && area < 1011);
        std::cout << "Passed!" << std::endl;
    }

    // Cut off from the Right
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from right test. Expected Area: 1010.96 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(90,50),20,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1010.9 && area < 1011);
        std::cout << "Passed!" << std::endl;
    }

    // Cut from top left
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from top left. Expected Area: 1753.19 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(10,20),30,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1753.1 && area < 1753.3);
        std::cout << "Passed!" << std::endl;
    }

    // Cut from top left
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from top right. Expected Area: 1753.19 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(90,20),30,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1753.1 && area < 1753.3);
        std::cout << "Passed!" << std::endl;
    }

    // Cut from top left
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from bottom left. Expected Area: 1753.19 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(10,80),30,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1753.1 && area < 1753.3);
        std::cout << "Passed!" << std::endl;
    }

    // Cut from top left
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from bottom right. Expected Area: 1753.19 " << std::endl;
        double area (analyzer.calculate_bordered_circle_area(QPoint(90,80),30,100,100));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 1753.1 && area < 1753.3);
        std::cout << "Passed!" << std::endl;
    }

    // Another random, more complicated scenario
    {
        RadialDistributionAnalyzer analyzer_2(1, 250, 1);
        std::cout << "*****************************************" << std::endl;
        std::cout << "Cut off from bottom right. Expected Area: 111149 " << std::endl;
        double area (analyzer_2.calculate_bordered_circle_area(QPoint(902,790),212,1000,1000));
        std::cout << "Calculated: " << area << std::endl;
        assert( area > 111148.9 );
        assert( area < 111149.5 );
        std::cout << "Passed!" << std::endl;
    }
}

// Integer read/write
#define TEST_FILE "/home/harry/Desktop/test_file"
void TestClass::test2()
{
    // 4 byte integer
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "4 BYTE INTEGER READ/WRITE TEST" << std::endl;
        int size(4);
        unsigned int value(65569);

        std::cout << "Writing/reading: " << value << std::endl;

        // Write
        std::ofstream out_stream;
        out_stream.open(TEST_FILE, std::ios_base::binary );
        unsigned char * bin_value(Binutils::toBin(value,size));
        out_stream.write((char*) bin_value, size);
        out_stream.close();

        // Read
        std::ifstream in_stream;
        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
        assert(in_stream.is_open());

        int file_size = in_stream.tellg();
        assert(file_size == size);

        in_stream.seekg(0, std::ios_base::beg);
        char * memblock = new char[size];
        in_stream.read(memblock, file_size);
        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
        in_stream.close();
        delete [] memblock;
        std::cout << "Read value : " << read_value << std::endl;
        assert(read_value == value);

        std::cout << "Passed!" << std::endl;
    }

    // 8 byte integer
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "8 BYTE INTEGER READ/WRITE TEST" << std::endl;
        int size(8);
        unsigned int value(65569);

        std::cout << "Writing/reading: " << value << std::endl;

        // Write
        std::ofstream out_stream;
        out_stream.open(TEST_FILE, std::ios_base::binary );
        unsigned char * bin_value(Binutils::toBin(value,size));
        out_stream.write((char*) bin_value, size);
        out_stream.close();

        // Read
        std::ifstream in_stream;
        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
        assert(in_stream.is_open());

        int file_size = in_stream.tellg();
        assert(file_size == size);

        in_stream.seekg(0, std::ios_base::beg);
        char * memblock = new char[size];
        in_stream.read(memblock, file_size);
        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
        in_stream.close();
        delete [] memblock;
        std::cout << "Read value : " << read_value << std::endl;
        assert(read_value == value);

        std::cout << "Passed!" << std::endl;
    }


    // 2 byte integer
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "2 BYTE INTEGER READ/WRITE TEST" << std::endl;
        int size(2);
        unsigned int value(65520);

        std::cout << "Writing/reading: " << value << std::endl;

        // Write
        std::ofstream out_stream;
        out_stream.open(TEST_FILE, std::ios_base::binary );
        unsigned char * bin_value(Binutils::toBin(value,size));
        out_stream.write((char*) bin_value, size);
        out_stream.close();

        // Read
        std::ifstream in_stream;
        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
        assert(in_stream.is_open());

        int file_size = in_stream.tellg();
        assert(file_size == size);

        in_stream.seekg(0, std::ios_base::beg);
        char * memblock = new char[size];
        in_stream.read(memblock, file_size);
        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
        in_stream.close();
        delete [] memblock;
        std::cout << "Read value : " << read_value << std::endl;
        assert(read_value == value);

        std::cout << "Passed!" << std::endl;
    }

    // 1 byte integer
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "1 BYTE INTEGER READ/WRITE TEST" << std::endl;
        int size(1);
        unsigned int value(135);

        std::cout << "Writing/reading: " << value << std::endl;

        // Write
        std::ofstream out_stream;
        out_stream.open(TEST_FILE, std::ios_base::binary );
        unsigned char * bin_value(Binutils::toBin(value,size));
        out_stream.write((char*) bin_value, size);
        out_stream.close();

        // Read
        std::ifstream in_stream;
        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
        assert(in_stream.is_open());

        int file_size = in_stream.tellg();
        assert(file_size == size);

        in_stream.seekg(0, std::ios_base::beg);
        char * memblock = new char[size];
        in_stream.read(memblock, file_size);
        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
        in_stream.close();
        delete [] memblock;
        std::cout << "Read value : " << read_value << std::endl;
        assert(read_value == value);

        std::cout << "Passed!" << std::endl;
    }
}

// Floating point read write
void TestClass::test3()
{
    // 4 byte float
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "4 BYTE FLOAT READ/WRITE TEST" << std::endl;
        int size(4);
        float value(12345.1234f);

        std::cout << "Writing/reading: " << value << std::endl;

        // Write
        std::ofstream out_stream;
        out_stream.open(TEST_FILE, std::ios_base::binary );
        unsigned char * bin_value(Binutils::toBin(value,size));
        out_stream.write((char*) bin_value, size);
        out_stream.close();

        // Read
        std::ifstream in_stream;
        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
        assert(in_stream.is_open());

        int file_size = in_stream.tellg();
        assert(file_size == size);

        in_stream.seekg(0, std::ios_base::beg);
        char * memblock = new char[size];
        in_stream.read(memblock, file_size);
        float read_value (Binutils::readFloat32((unsigned char*) memblock,file_size));
        in_stream.close();
        delete [] memblock;
        std::cout << "Read value : " << read_value << std::endl;
        assert(read_value >= value-.1f);
        assert(read_value <= value+.1f);

        std::cout << "Passed!" << std::endl;
    }

    // 8 byte float
    {
        std::cout << "*****************************************" << std::endl;
        std::cout << "8 BYTE FLOAT READ/WRITE TEST" << std::endl;
        int size(8);
        float value(12345.1234f);

        std::cout << "Writing/reading: " << value << std::endl;

        // Write
        std::ofstream out_stream;
        out_stream.open(TEST_FILE, std::ios_base::binary );
        unsigned char * bin_value(Binutils::toBin(value,size));
        out_stream.write((char*) bin_value, size);
        out_stream.close();

        // Read
        std::ifstream in_stream;
        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
        assert(in_stream.is_open());

        int file_size = in_stream.tellg();
        assert(file_size == size);

        in_stream.seekg(0, std::ios_base::beg);
        char * memblock = new char[size];
        in_stream.read(memblock, file_size);
        float read_value (Binutils::readFloat32((unsigned char*) memblock,file_size));
        in_stream.close();
        delete [] memblock;
        std::cout << "Read value : " << read_value << std::endl;
        assert(read_value >= value-.1f);
        assert(read_value <= value+.1f);

        std::cout << "Passed!" << std::endl;
    }
}

//int main(int argc, char *argv[])
//{
//    TestClass tc;
//    tc.runTests();
//    return 0;
//}

