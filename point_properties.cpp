#include "point_properties.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>

PointSizeProperties::PointSizeProperties(PointSizePropertiesHeader header, Histogram data) :
    m_header(header), m_data(data)
{

}

PointSizeProperties::PointSizeProperties(std::string filename)
{
    if(!load_data(filename))
    {
        std::cerr << "Failed to load data from file: " << filename << std::endl;
        exit(1);
    }
}

bool PointSizeProperties::load_data(std::string filename)
{
    std::ifstream file;
    file.open(filename, std::ios_base::binary | std::ios_base::ate);

    if(file.is_open())
    {
        std::streampos size;
        size = file.tellg();

        file.seekg(0, std::ios_base::beg);

        // Check the signature
        {
            char * memblock = new char[SIGNATURE_LENGTH];
            file.read(memblock, SIGNATURE_LENGTH);

            if(strcmp(memblock, SIGNATURE) != 0)
            {
                std::cerr << "File signature (" << memblock << ") is invalid!" << std::endl;
                delete [] memblock;
                return false;
            }

            delete [] memblock;
        }

        // Header
        {
            char * memblock = new char[4];

            // Points id
            file.read(memblock, 4);
            m_header.points_id = Binutils::readInt32((unsigned char*) memblock);

            // Min
            file.read(memblock, 4);
            m_header.min = Binutils::readInt32((unsigned char*) memblock);

            // Max
            file.read(memblock, 4);
            m_header.max = Binutils::readInt32((unsigned char*) memblock);

            // Diff
            file.read(memblock, 4);
            m_header.diff = Binutils::readInt32((unsigned char*) memblock);

            // N points
            file.read(memblock, 4);
            m_header.n_points = Binutils::readInt32((unsigned char*) memblock);

            delete [] memblock;
        }

        // Data
        {
            char * memblock = new char[4];
            unsigned int size;
            float ratio;

            while(file.tellg() < size)
            {
                // size
                file.read(memblock, 4);
                size = Binutils::readInt32((unsigned char*) memblock,4);

                // Distribution
                file.read(memblock, 4);
                ratio = Binutils::readFloat32((unsigned char*) memblock,4);

                m_data.insert(std::pair<int,float>(size, ratio));
            }

            delete [] memblock;
        }

        file.close();
        return true;
    }

    return false;
}

void PointSizeProperties::write(std::string filename)
{
    std::ofstream file;
    file.open(filename, std::ios_base::binary );

    // First write the header
    char * header (SIGNATURE);
    file.write(header, SIGNATURE_LENGTH);
    file.write((char*) Binutils::toBin((unsigned int)m_header.points_id,4),4);
    file.write((char*) Binutils::toBin((unsigned int)m_header.min,4),4);
    file.write((char*) Binutils::toBin((unsigned int)m_header.max,4),4);
    file.write((char*) Binutils::toBin((unsigned int)m_header.diff,4),4);
    file.write((char*) Binutils::toBin((unsigned int)m_header.n_points,4),4);


    for(std::pair<int,float> size_data : m_data)
    {
        file.write((char*) Binutils::toBin((unsigned int) size_data.first,4),4);
        file.write((char*) Binutils::toBin(size_data.second,4),4);
    }

    file.close();
}

void PointSizeProperties::writeToCSV(std::string filename)
{
    std::ofstream file;
    file.open(filename);

    if(file.is_open())
    {
        // First write the header
        file << "Points id," << m_header.points_id << "\n";
        file << "Min," << m_header.min << "\n";
        file << "Max," << m_header.max << "\n";
        file << "diff," << m_header.diff << "\n";
        file << "# points," << m_header.n_points << "\n";
        file << "\n";
        for(std::pair<int,float> point : m_data)
            file << point.first << "," << point.second << "\n";
    }
    file.close();
}
