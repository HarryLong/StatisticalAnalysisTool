#include "category_properties.h"
#include "utils/file_utils.h"
#include <iostream>
#include <fstream>
#include <cstring>

CategoryProperties::CategoryProperties(CategoryPropertiesHeader header, Histogram data) :
    m_header(header), m_data(data)
{

}

CategoryProperties::CategoryProperties(std::string filename)
{
    if(!load_data(filename))
    {
        std::cerr << "Failed to load data from file: " << filename << std::endl;
        exit(1);
    }
}

CategoryProperties::CategoryProperties()
{

}

bool CategoryProperties::load_data(std::string filename)
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
            char * memblock = new char[CATEGORY_PROPERTIES_SIGNATURE_LENGTH+1];
            file.read(memblock, CATEGORY_PROPERTIES_SIGNATURE_LENGTH);            
            memblock[CATEGORY_PROPERTIES_SIGNATURE_LENGTH] = '\0'; // Null-terminate

            std::string expected_signature(CATEGORY_PROPERTIES_SIGNATURE);
            if(strcmp(memblock, expected_signature.c_str()) != 0)
            {
                std::cerr << "File signature (" << memblock << ") is invalid! (Expected: " << CATEGORY_PROPERTIES_SIGNATURE << ")" << std::endl;
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
            m_header.category_id = FileUtils::readInt32((unsigned char*) memblock);

            // Diff
            file.read(memblock, 4);
            m_header.bin_size = FileUtils::readInt32((unsigned char*) memblock);

            // N points
            file.read(memblock, 4);
            m_header.n_points = FileUtils::readInt32((unsigned char*) memblock);

            // Priority
            file.read(memblock, 4);
            m_header.priority = FileUtils::readInt32((unsigned char*) memblock);

            // Height to radius multiplier
            file.read(memblock, 4);
            m_header.height_to_radius_multiplier = FileUtils::readFloat32((unsigned char*) memblock);

            // Height to root size multiplier
            file.read(memblock, 4);
            m_header.height_to_root_size_multiplier = FileUtils::readFloat32((unsigned char*) memblock);

            /**********
             * HEIGHT *
             **********/
            // Min
            file.read(memblock, 4);
            m_header.height_properties.min = FileUtils::readInt32((unsigned char*) memblock);

            // Max
            file.read(memblock, 4);
            m_header.height_properties.max = FileUtils::readInt32((unsigned char*) memblock);

            // Avg
            file.read(memblock, 4);
            m_header.height_properties.avg = FileUtils::readFloat32((unsigned char*) memblock);

            // Standard Deviation
            file.read(memblock, 4);
            m_header.height_properties.standard_dev = FileUtils::readFloat32((unsigned char*) memblock);

            /*****************
             * CANOPY RADIUS *
             *****************/
            // Min
            file.read(memblock, 4);
            m_header.radius_properties.min = FileUtils::readInt32((unsigned char*) memblock);

            // Max
            file.read(memblock, 4);
            m_header.radius_properties.max = FileUtils::readInt32((unsigned char*) memblock);

            // Avg
            file.read(memblock, 4);
            m_header.radius_properties.avg = FileUtils::readFloat32((unsigned char*) memblock);

            // Standard Deviation
            file.read(memblock, 4);
            m_header.radius_properties.standard_dev = FileUtils::readFloat32((unsigned char*) memblock);

            /*************
             * ROOT SIZE *
             *************/
            // Min
            file.read(memblock, 4);
            m_header.root_size_properties.min = FileUtils::readInt32((unsigned char*) memblock);

            // Max
            file.read(memblock, 4);
            m_header.root_size_properties.max = FileUtils::readInt32((unsigned char*) memblock);

            // Avg
            file.read(memblock, 4);
            m_header.root_size_properties.avg = FileUtils::readFloat32((unsigned char*) memblock);

            // Standard Deviation
            file.read(memblock, 4);
            m_header.root_size_properties.standard_dev = FileUtils::readFloat32((unsigned char*) memblock);

            // Dependent category ids
            {
                file.read(memblock, 4);
                int n_dependent_category_ids (FileUtils::readInt32((unsigned char*) memblock) );
                for(int i (0); i < n_dependent_category_ids; i++)
                {
                    file.read(memblock, 4);
                    m_header.category_dependent_ids.insert( FileUtils::readInt32((unsigned char*) memblock) );
                }
            }

            delete [] memblock;
        }

        // Data
        {
            char * memblock = new char[4];
            unsigned int point_size;
            float ratio;

            while(file.tellg() < size)
            {
                // size
                file.read(memblock, 4);
                point_size = FileUtils::readInt32((unsigned char*) memblock,4);

                // Distribution
                file.read(memblock, 4);
                ratio = FileUtils::readFloat32((unsigned char*) memblock,4);

                m_data.insert(std::pair<int,float>(point_size, ratio));
            }

            delete [] memblock;
        }

        file.close();
        return true;
    }

    return false;
}

void CategoryProperties::write(std::string filename)
{
    std::ofstream file;
    file.open(filename, std::ios_base::binary | std::ios_base::trunc );

    // First write the header
    std::string header (CATEGORY_PROPERTIES_SIGNATURE);
    file.write(header.c_str(), CATEGORY_PROPERTIES_SIGNATURE_LENGTH);
    file.write((char*) FileUtils::toBin((unsigned int)m_header.category_id,4),4);

    file.write((char*) FileUtils::toBin((unsigned int)m_header.bin_size,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)m_header.n_points,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)m_header.priority,4),4);
    file.write((char*) FileUtils::toBin(m_header.height_to_radius_multiplier,4),4);
    file.write((char*) FileUtils::toBin(m_header.height_to_root_size_multiplier,4),4);
    //
    /**********
     * HEIGHT *
     **********/
    file.write((char*) FileUtils::toBin((unsigned int)m_header.height_properties.min,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)m_header.height_properties.max,4),4);
    file.write((char*) FileUtils::toBin(m_header.height_properties.avg,4),4);
    file.write((char*) FileUtils::toBin(m_header.height_properties.standard_dev,4),4);
    /*****************
     * CANOPY RADIUS *
     *****************/
    file.write((char*) FileUtils::toBin((unsigned int)m_header.radius_properties.min,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)m_header.radius_properties.max,4),4);
    file.write((char*) FileUtils::toBin(m_header.radius_properties.avg,4),4);
    file.write((char*) FileUtils::toBin(m_header.radius_properties.standard_dev,4),4);
    /*************
     * ROOT SIZE *
     *************/
    file.write((char*) FileUtils::toBin((unsigned int)m_header.root_size_properties.min,4),4);
    file.write((char*) FileUtils::toBin((unsigned int)m_header.root_size_properties.max,4),4);
    file.write((char*) FileUtils::toBin(m_header.root_size_properties.avg,4),4);
    file.write((char*) FileUtils::toBin(m_header.root_size_properties.standard_dev,4),4);
    //
    // Dependent category ids
    file.write((char*) FileUtils::toBin((unsigned int)m_header.category_dependent_ids.size(),4),4);
    for(int category_dependent_id : m_header.category_dependent_ids)
        file.write((char*) FileUtils::toBin((unsigned int)category_dependent_id,4),4);

    for(std::pair<int,float> size_data : m_data)
    {
        file.write((char*) FileUtils::toBin((unsigned int) size_data.first,4),4);
        file.write((char*) FileUtils::toBin(size_data.second,4),4);
    }

    file.close();
}

void CategoryProperties::writeToCSV(std::string filename)
{
    std::ofstream file;
    file.open(filename);

    if(file.is_open())
    {
        // First write the header
        file << "Points id," << m_header.category_id << "\n";
        file << "priority," << m_header.priority<< "\n";
        file << "diff," << m_header.bin_size << "\n";
        file << "# points," << m_header.n_points << "\n";
        file << "height to canopy width multiplier," << m_header.height_to_radius_multiplier << "\n";
        file << "height to root size multiplier," << m_header.height_to_root_size_multiplier << "\n";
        //CANOPY
        file << "Min radius," << m_header.radius_properties.min << "\n";
        file << "Max radius," << m_header.radius_properties.max << "\n";
        file << "Avg radius," << m_header.radius_properties.avg << "\n";
        file << "Standard dev radius," << m_header.radius_properties.standard_dev << "\n";
        // HEIGHT
        file << "Min height," << m_header.height_properties.min << "\n";
        file << "Max height," << m_header.height_properties.max << "\n";
        file << "Avg height," << m_header.height_properties.avg << "\n";
        file << "Standard dev height," << m_header.height_properties.standard_dev << "\n";
        // ROOT SIZE
        file << "Root size min," << m_header.root_size_properties.min << "\n";
        file << "Root size max," << m_header.root_size_properties.max << "\n";
        file << "Avg root size," << m_header.root_size_properties.avg << "\n";
        file << "Standard dev root size," << m_header.root_size_properties.standard_dev << "\n";

        // Dependent category ids
        file << "Dependent category ids, ";
        for(int category_dependent_id : m_header.category_dependent_ids)
            file << category_dependent_id << ", ";
        file << "\n\n";
        for(std::pair<int,float> point : m_data)
            file << point.first << "," << point.second << "\n";
    }
    file.close();
}
