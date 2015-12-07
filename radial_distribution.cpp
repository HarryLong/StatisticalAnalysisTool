#include "radial_distribution.h"
#include "utils/file_utils.h"
#include <iostream>
#include <fstream>
#include <cstring>

std::string RadialDistributionHeader::_SIGNATURE = "RADIALDISTRIBUTIONFILE";

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
    const char * header  = RadialDistributionHeader::_SIGNATURE.c_str();
    file.write(header, RadialDistributionHeader::_SIGNATURE.length());
    file.write((char*) FileUtils::toBin(m_header.reference_id,4),4);
    file.write((char*) FileUtils::toBin(m_header.destination_id,4),4);
    file.write((char*) FileUtils::toBin((unsigned int) (m_header.requires_optimization ? 1 : 0),4),4);

    // DATA
    file.write((char*) FileUtils::toBin(m_within_radius_distribution,4),4);
    file.write((char*) FileUtils::toBin(m_past_rmax_distribution,4),4);

    for(std::pair<int,float> point : m_data)
    {
        file.write((char*) FileUtils::toBin((unsigned int) point.first,4),4);
        file.write((char*) FileUtils::toBin(point.second,4),4);
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
            int sig_length(RadialDistributionHeader::_SIGNATURE.length());
            char * memblock = new char[sig_length+1];
            file.read(memblock, sig_length);
            memblock[sig_length] = '\0'; // Null-terminate

            std::string expected_signature(RadialDistributionHeader::_SIGNATURE);
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
            m_header.reference_id = FileUtils::readInt32((unsigned char*) memblock);

            // Destination id
            file.read(memblock, 4);
            m_header.destination_id = FileUtils::readInt32((unsigned char*) memblock);

            // Requires optimization
            file.read(memblock, 4);
            m_header.requires_optimization = (FileUtils::readInt32((unsigned char*) memblock) == 1 ? true : false);

            delete [] memblock;
        }

        // Data
        {
            char * memblock = new char[4];
            unsigned int r;
            float distribution;

            // First the within radius distribution
            file.read(memblock, 4);
            m_within_radius_distribution = FileUtils::readFloat32((unsigned char*) memblock,4);

            // First the within radius distribution
            file.read(memblock, 4);
            m_past_rmax_distribution = FileUtils::readFloat32((unsigned char*) memblock,4);

            while(file.tellg() < size)
            {
                // R
                file.read(memblock, 4);
                r = FileUtils::readInt32((unsigned char*) memblock,4);

                // Distribution
                file.read(memblock, 4);
                distribution = FileUtils::readFloat32((unsigned char*) memblock,4);

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
    std::cout << "Requires optimization: " << (m_header.requires_optimization ? "Yes" : "No") << std::endl;
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
        file << "Requires optimization, " << (m_header.requires_optimization ? "Yes" : "No") << "\n";
        file << "\n";
        file << "Within radius," << m_within_radius_distribution << "\n";
        file << "Past rmax," << m_past_rmax_distribution << "\n";
        for(std::pair<int,float> point : m_data)
            file << point.first << "," << point.second << "\n";
    }
    file.close();
}
