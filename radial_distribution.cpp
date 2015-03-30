#include "radial_distribution.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>

RadialDistribution::RadialDistribution(RadialDistributionPorperties properties, Histogram data) :
    m_properties(properties), m_data(data)
{

}

RadialDistribution::RadialDistribution(std::string filename)
{
    if(!load(filename))
    {
        std::cerr << "Invalid radial distribution file: " << filename << std::endl;
        exit(1);
    }
}

void RadialDistribution::write(std::string filename)
{
    std::ofstream file;
    file.open(filename, std::ios_base::binary );

    // First write the header
    char * header (SIGNATURE);
    file.write(header, SIGNATURE_LENGTH);
    file.write((char*) Binutils::toBin(m_properties.reference_id,4),4);
    file.write((char*) Binutils::toBin(m_properties.destination_id,4),4);
    file.write((char*) Binutils::toBin(m_properties.n_reference_points,4),4);
    file.write((char*) Binutils::toBin(m_properties.n_destination_points,4),4);
    file.write((char*) Binutils::toBin(m_properties.analysed_area,4),4);
    file.write((char*) Binutils::toBin(m_properties.r_min,4),4);
    file.write((char*) Binutils::toBin(m_properties.r_max,4),4);
    file.write((char*) Binutils::toBin(m_properties.r_diff,4),4);

    for(std::pair<int,float> point : m_data)
    {
        file.write((char*) Binutils::toBin((unsigned int) point.first,4),4);
        file.write((char*) Binutils::toBin(point.second,4),4);
    }

    file.close();
}

bool RadialDistribution::load(std::string filename)
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

            // Reference id
            file.read(memblock, 4);
            m_properties.reference_id = Binutils::readInt32((unsigned char*) memblock);

            // Destination id
            file.read(memblock, 4);
            m_properties.destination_id = Binutils::readInt32((unsigned char*) memblock);

            // # Reference points
            file.read(memblock, 4);
            m_properties.n_reference_points = Binutils::readInt32((unsigned char*) memblock);

            // # Destination points
            file.read(memblock, 4);
            m_properties.n_destination_points = Binutils::readInt32((unsigned char*) memblock);

            // Analysed area
            file.read(memblock, 4);
            m_properties.analysed_area = Binutils::readInt32((unsigned char*) memblock);

            // R min
            file.read(memblock, 4);
            m_properties.r_min = Binutils::readInt32((unsigned char*) memblock);

            // R max
            file.read(memblock, 4);
            m_properties.r_max = Binutils::readInt32((unsigned char*) memblock);

            // R diff
            file.read(memblock, 4);
            m_properties.r_diff = Binutils::readInt32((unsigned char*) memblock);

            delete [] memblock;
        }

        // Data
        {
            char * memblock = new char[4];
            unsigned int r;
            float distribution;

            while(file.tellg() < size)
            {
                // R
                file.read(memblock, 4);
                r = Binutils::readInt32((unsigned char*) memblock,4);

                // Distribution
                file.read(memblock, 4);
                distribution = Binutils::readFloat32((unsigned char*) memblock,4);

                m_data.insert(std::pair<int,float>(r, distribution));
            }

            delete [] memblock;
        }

        file.close();
        return true;
    }

    return false;
}

void RadialDistribution::printToConsole()
{
    std::cout << "***********HEADER*****************" << std::endl;
    std::cout << "Reference id: " << m_properties.reference_id << std::endl;
    std::cout << "Destination id: " << m_properties.destination_id << std::endl;
    std::cout << "# reference points: " << m_properties.n_reference_points << std::endl;
    std::cout << "# destination points: " << m_properties.n_destination_points << std::endl;
    std::cout << "Analysed area: " << m_properties.analysed_area << std::endl;
    std::cout << "R min: " << m_properties.r_min << std::endl;
    std::cout << "R max: " << m_properties.r_max << std::endl;
    std::cout << "R diff: " << m_properties.r_diff << std::endl;
    std::cout << "************DATA******************" << std::endl;
    for(std::pair<int,float> point : m_data)
        std::cout << "R: " << point.first << " | Variance: " << point.second << std::endl;
}


void RadialDistribution::writeToCSV(std::string filename)
{
    std::ofstream file;
    file.open(filename);

    if(file.is_open())
    {
        // First write the header
        file << "Reference id," << m_properties.reference_id << "\n";
        file << "Destination id," << m_properties.destination_id << "\n";
        file << "# reference points," << m_properties.n_reference_points << "\n";
        file << "# destination points," << m_properties.n_destination_points << "\n";
        file << "Analysed area," << m_properties.analysed_area << "\n";
        file << "R min," << m_properties.r_min << "\n";
        file << "R max," << m_properties.r_max << "\n";
        file << "R diff," << m_properties.r_diff << "\n";
        file << "\n";
        for(std::pair<int,float> point : m_data)
            file << point.first << "," << point.second << "\n";
    }
    file.close();
}
