#include "radial_distribution.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>

RadialDistribution::RadialDistribution(RadialDistributionHeader header, float within_radius_distribution, float past_rmax_distribution, Histogram data) :
    m_header(header), m_data(data), m_within_radius_distribution(within_radius_distribution), m_past_rmax_distribution(past_rmax_distribution),
    m_maximum(-1), m_minimum(-1)
{
    calculate_min_max();
}

RadialDistribution::RadialDistribution(std::string filename)
{
    if(!load(filename))
    {
        std::cerr << "Invalid radial distribution file: " << filename << std::endl;
        exit(1);
    }
    calculate_min_max();
}

RadialDistribution::RadialDistribution()
{

}

void RadialDistribution::calculate_min_max()
{
    // MIN
    {
        m_minimum = m_within_radius_distribution;
        for(auto it(m_data.begin()); it != m_data.end(); it++)
        {
            if(it->second < m_minimum)
                m_minimum = it->second;
        }
        if(m_past_rmax_distribution < m_minimum)
            m_minimum = m_past_rmax_distribution;
    }
    // MAX
    {
        m_maximum = m_within_radius_distribution;
        for(auto it(m_data.begin()); it != m_data.end(); it++)
        {
            if(it->second > m_maximum)
                m_maximum = it->second;
        }
        if(m_past_rmax_distribution > m_maximum)
            m_maximum = m_past_rmax_distribution;
    }

}

float RadialDistribution::getMaximum() const
{
    return m_maximum;
}

float RadialDistribution::getMinimum() const
{
    return m_minimum;
}

void RadialDistribution::write(std::string filename) const
{
    std::ofstream file;
    file.open(filename, std::ios_base::binary | std::ios_base::trunc );

    // First write the header
    char * header (RADIAL_DISTRIBUTION_SIGNATURE);
    file.write(header, RADIAL_DISTRIBUTION_SIGNATURE_LENGTH);
    file.write((char*) Binutils::toBin(m_header.reference_id,4),4);
    file.write((char*) Binutils::toBin(m_header.destination_id,4),4);

    file.write((char*) Binutils::toBin(m_within_radius_distribution,4),4);
    file.write((char*) Binutils::toBin(m_past_rmax_distribution,4),4);

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
            char * memblock = new char[RADIAL_DISTRIBUTION_SIGNATURE_LENGTH+1];
            file.read(memblock, RADIAL_DISTRIBUTION_SIGNATURE_LENGTH);
            memblock[RADIAL_DISTRIBUTION_SIGNATURE_LENGTH] = '\0'; // Null-terminate

            std::string expected_signature(RADIAL_DISTRIBUTION_SIGNATURE);
            if(strcmp(memblock, expected_signature.c_str()) != 0)
            {
                std::cerr << "File signature (" << memblock << ") is invalid! (Expected: " << expected_signature << ")" << std::endl;
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
            m_header.reference_id = Binutils::readInt32((unsigned char*) memblock);

            // Destination id
            file.read(memblock, 4);
            m_header.destination_id = Binutils::readInt32((unsigned char*) memblock);

            delete [] memblock;
        }

        // Data
        {
            char * memblock = new char[4];
            unsigned int r;
            float distribution;

            // First the within radius distribution
            file.read(memblock, 4);
            m_within_radius_distribution = Binutils::readFloat32((unsigned char*) memblock,4);

            // First the within radius distribution
            file.read(memblock, 4);
            m_past_rmax_distribution = Binutils::readFloat32((unsigned char*) memblock,4);

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

void RadialDistribution::printToConsole() const
{
    std::cout << "***********HEADER*****************" << std::endl;
    std::cout << "Reference id: " << m_header.reference_id << std::endl;
    std::cout << "Destination id: " << m_header.destination_id << std::endl;
    std::cout << "************DATA******************" << std::endl;
    std::cout << "Within radius: " << m_within_radius_distribution << std::endl;
    std::cout << "Past radius: " << m_past_rmax_distribution << std::endl;
    for(std::pair<int,float> point : m_data)
        std::cout << "R: " << point.first << " | Variance: " << point.second << std::endl;
}


void RadialDistribution::writeToCSV(std::string filename) const
{
    std::ofstream file;
    file.open(filename);

    if(file.is_open())
    {
        // First write the header
        file << "Reference id," << m_header.reference_id << "\n";
        file << "Destination id," << m_header.destination_id << "\n";
        file << "\n";
        file << "Within radius," << m_within_radius_distribution << "\n";
        file << "Past rmax," << m_past_rmax_distribution << "\n";
        for(std::pair<int,float> point : m_data)
            file << point.first << "," << point.second << "\n";
    }
    file.close();
}
