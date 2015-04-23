#include "unit_tests.h"
#include "radial_distribution_analyzer.h"
#include "distribution_factory.h"
#include <cassert>
#include <iostream>
#include "utils.h"
#include <iostream>
#include <fstream>
#include "category_analyzer.h"

TestClass::TestClass()
{

}

void TestClass::runTests()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}

void TestClass::test1() {}
//{
//    std::cout << "Testing bordered circles area calculations..." << std::endl;
//    RadialDistributionAnalyzer analyzer(1000,1000, 1, 100, 1);

//    // Cut off from the top
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from top test. Expected Area: 1010.96 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(50,10),20,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1010.9 && area < 1011);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut off from the bottom
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from botom test. Expected Area: 1010.96 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(50,90),20,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1010.9 && area < 1011);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut off from the left
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from left test. Expected Area: 1010.96 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(10,50),20,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1010.9 && area < 1011);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut off from the Right
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from right test. Expected Area: 1010.96 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(90,50),20,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1010.9 && area < 1011);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut from top left
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from top left. Expected Area: 1753.19 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(10,20),30,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1753.1 && area < 1753.3);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut from top left
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from top right. Expected Area: 1753.19 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(90,20),30,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1753.1 && area < 1753.3);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut from top left
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from bottom left. Expected Area: 1753.19 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(10,80),30,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1753.1 && area < 1753.3);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Cut from top left
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from bottom right. Expected Area: 1753.19 " << std::endl;
//        double area (analyzer.calculate_bordered_circle_area(QPoint(90,80),30,100,100));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 1753.1 && area < 1753.3);
//        std::cout << "Passed!" << std::endl;
//    }

//    // Another random, more complicated scenario
//    {
//        RadialDistributionAnalyzer analyzer_2(1000,1000,1, 250, 1);
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "Cut off from bottom right. Expected Area: 111149 " << std::endl;
//        double area (analyzer_2.calculate_bordered_circle_area(QPoint(902,790),212,1000,1000));
//        std::cout << "Calculated: " << area << std::endl;
//        assert( area > 111148.9 );
//        assert( area < 111149.5 );
//        std::cout << "Passed!" << std::endl;
//    }
//}

// Integer read/write
#define TEST_FILE "/home/harry/Desktop/test_file"
void TestClass::test2(){}
//{
//    // 4 byte integer
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "4 BYTE INTEGER READ/WRITE TEST" << std::endl;
//        int size(4);
//        unsigned int value(65569);

//        std::cout << "Writing/reading: " << value << std::endl;

//        // Write
//        std::ofstream out_stream;
//        out_stream.open(TEST_FILE, std::ios_base::binary );
//        unsigned char * bin_value(Binutils::toBin(value,size));
//        out_stream.write((char*) bin_value, size);
//        out_stream.close();

//        // Read
//        std::ifstream in_stream;
//        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
//        assert(in_stream.is_open());

//        int file_size = in_stream.tellg();
//        assert(file_size == size);

//        in_stream.seekg(0, std::ios_base::beg);
//        char * memblock = new char[size];
//        in_stream.read(memblock, file_size);
//        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
//        in_stream.close();
//        delete [] memblock;
//        std::cout << "Read value : " << read_value << std::endl;
//        assert(read_value == value);

//        std::cout << "Passed!" << std::endl;
//    }

//    // 8 byte integer
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "8 BYTE INTEGER READ/WRITE TEST" << std::endl;
//        int size(8);
//        unsigned int value(65569);

//        std::cout << "Writing/reading: " << value << std::endl;

//        // Write
//        std::ofstream out_stream;
//        out_stream.open(TEST_FILE, std::ios_base::binary );
//        unsigned char * bin_value(Binutils::toBin(value,size));
//        out_stream.write((char*) bin_value, size);
//        out_stream.close();

//        // Read
//        std::ifstream in_stream;
//        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
//        assert(in_stream.is_open());

//        int file_size = in_stream.tellg();
//        assert(file_size == size);

//        in_stream.seekg(0, std::ios_base::beg);
//        char * memblock = new char[size];
//        in_stream.read(memblock, file_size);
//        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
//        in_stream.close();
//        delete [] memblock;
//        std::cout << "Read value : " << read_value << std::endl;
//        assert(read_value == value);

//        std::cout << "Passed!" << std::endl;
//    }


//    // 2 byte integer
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "2 BYTE INTEGER READ/WRITE TEST" << std::endl;
//        int size(2);
//        unsigned int value(65520);

//        std::cout << "Writing/reading: " << value << std::endl;

//        // Write
//        std::ofstream out_stream;
//        out_stream.open(TEST_FILE, std::ios_base::binary );
//        unsigned char * bin_value(Binutils::toBin(value,size));
//        out_stream.write((char*) bin_value, size);
//        out_stream.close();

//        // Read
//        std::ifstream in_stream;
//        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
//        assert(in_stream.is_open());

//        int file_size = in_stream.tellg();
//        assert(file_size == size);

//        in_stream.seekg(0, std::ios_base::beg);
//        char * memblock = new char[size];
//        in_stream.read(memblock, file_size);
//        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
//        in_stream.close();
//        delete [] memblock;
//        std::cout << "Read value : " << read_value << std::endl;
//        assert(read_value == value);

//        std::cout << "Passed!" << std::endl;
//    }

//    // 1 byte integer
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "1 BYTE INTEGER READ/WRITE TEST" << std::endl;
//        int size(1);
//        unsigned int value(135);

//        std::cout << "Writing/reading: " << value << std::endl;

//        // Write
//        std::ofstream out_stream;
//        out_stream.open(TEST_FILE, std::ios_base::binary );
//        unsigned char * bin_value(Binutils::toBin(value,size));
//        out_stream.write((char*) bin_value, size);
//        out_stream.close();

//        // Read
//        std::ifstream in_stream;
//        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
//        assert(in_stream.is_open());

//        int file_size = in_stream.tellg();
//        assert(file_size == size);

//        in_stream.seekg(0, std::ios_base::beg);
//        char * memblock = new char[size];
//        in_stream.read(memblock, file_size);
//        int read_value (Binutils::readInt32((unsigned char*) memblock,file_size));
//        in_stream.close();
//        delete [] memblock;
//        std::cout << "Read value : " << read_value << std::endl;
//        assert(read_value == value);

//        std::cout << "Passed!" << std::endl;
//    }
//}

// Floating point read write
void TestClass::test3() {}
//{
//    // 4 byte float
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "4 BYTE FLOAT READ/WRITE TEST" << std::endl;
//        int size(4);
//        float value(12345.1234f);

//        std::cout << "Writing/reading: " << value << std::endl;

//        // Write
//        std::ofstream out_stream;
//        out_stream.open(TEST_FILE, std::ios_base::binary );
//        unsigned char * bin_value(Binutils::toBin(value,size));
//        out_stream.write((char*) bin_value, size);
//        out_stream.close();

//        // Read
//        std::ifstream in_stream;
//        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
//        assert(in_stream.is_open());

//        int file_size = in_stream.tellg();
//        assert(file_size == size);

//        in_stream.seekg(0, std::ios_base::beg);
//        char * memblock = new char[size];
//        in_stream.read(memblock, file_size);
//        float read_value (Binutils::readFloat32((unsigned char*) memblock,file_size));
//        in_stream.close();
//        delete [] memblock;
//        std::cout << "Read value : " << read_value << std::endl;
//        assert(read_value >= value-.1f);
//        assert(read_value <= value+.1f);

//        std::cout << "Passed!" << std::endl;
//    }

//    // 8 byte float
//    {
//        std::cout << "*****************************************" << std::endl;
//        std::cout << "8 BYTE FLOAT READ/WRITE TEST" << std::endl;
//        int size(8);
//        float value(12345.1234f);

//        std::cout << "Writing/reading: " << value << std::endl;

//        // Write
//        std::ofstream out_stream;
//        out_stream.open(TEST_FILE, std::ios_base::binary );
//        unsigned char * bin_value(Binutils::toBin(value,size));
//        out_stream.write((char*) bin_value, size);
//        out_stream.close();

//        // Read
//        std::ifstream in_stream;
//        in_stream.open(TEST_FILE, std::ios_base::binary | std::ios_base::ate);
//        assert(in_stream.is_open());

//        int file_size = in_stream.tellg();
//        assert(file_size == size);

//        in_stream.seekg(0, std::ios_base::beg);
//        char * memblock = new char[size];
//        in_stream.read(memblock, file_size);
//        float read_value (Binutils::readFloat32((unsigned char*) memblock,file_size));
//        in_stream.close();
//        delete [] memblock;
//        std::cout << "Read value : " << read_value << std::endl;
//        assert(read_value >= value-.1f);
//        assert(read_value <= value+.1f);

//        std::cout << "Passed!" << std::endl;
//    }
//}

// Test writing and loading from a radial distribution file
void TestClass::test4() {}
//{
//    std::cout << "*****************************************" << std::endl;
//    std::cout << "READ/WRITE TO RADIAL DISTRIBUTION FILE" << std::endl;

//    DistributionFactory factory;
//    std::vector<AnalysisPoint*> points(factory.generateRandomDistribution(0,1000, 500, 500));

//    RadialDistribution::Histogram original_data;
//    RadialDistributionHeader original_properties;

//    // Original
//    {
//        RadialDistributionAnalyzer analyzer(500,500,0, 200, 10);
//        RadialDistribution distribution(analyzer.getRadialDistribution(points, points,0,0));
//        original_data = distribution.m_data;
//        original_properties = distribution.m_header;

//        // Write to test file
//        distribution.write(TEST_FILE);
//    }


//    RadialDistribution::Histogram loaded_data;
//    RadialDistributionHeader loaded_properties;
//    {
//        RadialDistribution distribution(TEST_FILE);
//        loaded_data = distribution.m_data;
//        loaded_properties = distribution.m_header;
//    }

//    {
//        std::cout << "Expected size: " << original_data.size() << std::flush;
//        std::cout << "Size: " << loaded_data.size() << std::flush;
//        assert(original_data.size() == loaded_data.size());
//        std::cout << "\tPassed!"<< std::endl;

//        for(auto it(original_data.begin()); it != original_data.end(); it++)
//        {
//            std::cout << "Bracket: " << it->first << " | Expected: " << it->second <<std::flush;

//            float loaded_value(loaded_data[it->first]);
//            std::cout << " | Loaded: " << loaded_value << std::endl;
//            assert(loaded_value > it->second-.01f && loaded_value < it->second+.01f);
//            std::cout << "\tPassed!"<< std::endl;
//        }
//    }
//}

void TestClass::test5() {}
//{
//    std::cout << "*****************************************" << std::endl;
//    std::cout << "READ/WRITE TO SIZE PROPERTIES FILE" << std::endl;

//    DistributionFactory factory;
//    std::vector<AnalysisPoint*> points(factory.generateRandomDistribution(0, 1000, 500, 500));

//    CategoryProperties::Histogram original_data;
//    CategoryPropertiesHeader original_header;

//    // Original
//    {
//        CategoryAnalyzer point_size_analyzer;
//        CategoryProperties properties( point_size_analyzer.getCategoryProperties(points,0) );
//        original_data = properties.m_data;
//        original_header = properties.m_header;

//        // Write to test file
//        properties.write(TEST_FILE);
//    }


//    CategoryProperties::Histogram loaded_data;
//    CategoryPropertiesHeader loaded_header;
//    {
//        CategoryProperties properties(TEST_FILE);
//        loaded_data = properties.m_data;
//        loaded_header = properties.m_header;
//    }

//    {
//        std::cout << "Expected size: " << original_data.size() << std::flush;
//        std::cout << "Size: " << loaded_data.size() << std::flush;
//        assert(original_data.size() == loaded_data.size());
//        std::cout << "\tPassed!"<< std::endl;

//        for(auto it(original_data.begin()); it != original_data.end(); it++)
//        {
//            std::cout << "Bracket: " << it->first << " | Expected: " << it->second <<std::flush;

//            float loaded_value(loaded_data[it->first]);
//            std::cout << " | Loaded: " << loaded_value << std::endl;
//            assert(loaded_value > it->second-.01f && loaded_value < it->second+.01f);
//            std::cout << "\tPassed!"<< std::endl;
//        }
//    }
//}

void TestClass::test6() {}
//{
//    std::cout << "*****************************************" << std::endl;
//    std::cout << "SIZE PROPERTIES ANALYZER TEST" << std::endl;

//    std::vector<AnalysisPoint*> points;
//    for(int i(1); i < 11; i++)
//        points.push_back(new AnalysisPoint(0, QPoint(i,i), i));

//    CategoryAnalyzer point_size_analyzer;
//    CategoryProperties properties = point_size_analyzer.getCategoryProperties(points, 55);

//    // Header
//    assert(properties.m_header.bin_size == 1);
//    assert(properties.m_header.min_size == 1);
//    assert(properties.m_header.bin_size == 1);
//    assert(properties.m_header.max_size == 10);
//    assert(properties.m_header.n_points == 10);
//    assert(properties.m_header.category_id == 55);

//    // Data
//    assert(properties.m_data.size() == 10);
//    for(int i(1); i < 11; i++)
//    {
//        std::cout << "Radius: " << i << " | Expected: 0.1 | Received: " << properties.m_data[i] << "..." << std::flush;
//        assert(properties.m_data[i] > 0.09 && properties.m_data[i] < 0.11);
//        std::cout << "OK!" << std::endl;
//    }

//    for(AnalysisPoint * p : points)
//        delete p;
//}
