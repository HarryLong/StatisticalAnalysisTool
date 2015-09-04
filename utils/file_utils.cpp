#include "file_utils.h"
#include <iostream>
#include <QDir>

#define RADIAL_DISTRIBUTION_SUBFOLDER "radial_distribution"
#define CATEGORY_PROPERTIES_SUBFOLDER "category_properties"
#define CSV_FILES_SUBFOLDER "csv"

#define CONFIGURATION_FILE_NAME "configuration.conf"
#define RADIAL_DISTRIBUTION_FILE_EXTENSION ".rad"
#define CATEGORY_PROPERTIES_FILE_EXTENSION ".category_properties"

#define MAX_8_BITS 256
#define MAX_16_BITS 65536

const char * FileUtils::_CONFIGURATION_FILENAME = CONFIGURATION_FILE_NAME;
const char * FileUtils::_RADIAL_DISTRIBUTION_EXT = RADIAL_DISTRIBUTION_FILE_EXTENSION;
const char * FileUtils::_CATEGORY_PROPERTIES_EXT = CATEGORY_PROPERTIES_FILE_EXTENSION;

/*************
 * BIN UTILS *
 *************/
void FileUtils::insertPadding(unsigned char * data, int from, int to)
{
    for(int i(from); i < to; i++)
        data[i] = 0x00;
}

unsigned char* FileUtils::toBin(unsigned int value, int n_bytes)
{
    unsigned char* ret = new unsigned char[n_bytes];
    int byte_index(0);
    // First the padding
    if(n_bytes > 4)
    {
        insertPadding(ret, 0, n_bytes-4);
        byte_index += (n_bytes-4);
    }

    // Now the data
    int remaining_bytes(std::min(n_bytes, 4));
    for(int i(0); i < remaining_bytes; i++)
    {
        unsigned char data ( (value >> ((remaining_bytes-1-i)*8)) & int(0x00000FF));
        ret[byte_index++] = data;
    }

    return ret;
}

/**
  Very simple modeling of floating point values where the first 16 bits represent the integral part and the next 16 bits represent
  the fractional part.
 * @brief Binutils::toBin
 * @param value
 * @param n_bytes
 * @return
 */
unsigned char* FileUtils::toBin(float value, int n_bytes)
{
    if(n_bytes < 4)
    {
        std::cerr << "Can't represent a float in " << n_bytes << " bytes";
        exit(1);
    }

    unsigned char* ret = new unsigned char[n_bytes];
    int byte_index(0);

    // first the padding
    // First the padding
    if(n_bytes > 4)
    {
        insertPadding(ret, 0, n_bytes-4);
        byte_index += (n_bytes-4);
    }

    // Now the data
    unsigned int integral_part(value);
    unsigned int fractional_part;
    {
        float tmp_fractional_part(value-integral_part + 1.0f);

        while((tmp_fractional_part*10) < MAX_16_BITS)
            tmp_fractional_part *= 10;
        fractional_part = (unsigned int) tmp_fractional_part;
    }

    unsigned char * bin_integral_part (toBin(integral_part,2)) ;
    unsigned char * bin_fractional_part (toBin(fractional_part, 2));

//    std::cout << "integral_part " << integral_part << std::endl;
//    std::cout << "fractional " << fractional_part << std::endl;

    ret[byte_index++] = bin_integral_part[0];
    ret[byte_index++] = bin_integral_part[1];
    ret[byte_index++] = bin_fractional_part[0];
    ret[byte_index++] = bin_fractional_part[1];

    delete [] bin_integral_part;
    delete [] bin_fractional_part;

    return ret;
}

int FileUtils::readInt32(unsigned char * data, int n_bytes)
{
    int value(0);

    int padding_bytes(std::max(0,n_bytes-4));

    int remaining_bytes(n_bytes-padding_bytes);
    int coefficient((remaining_bytes-1)*8);
    for(int i(0); i < remaining_bytes; i++,coefficient -= 8)
    {
        value += (data[i+padding_bytes] * pow(2, coefficient));
    }

    return value;
}

float FileUtils::readFloat32(unsigned char * data, int n_bytes)
{
    if(n_bytes < 4)
    {
        std::cerr << "Can't represent a float in " << n_bytes << " bytes";
        exit(1);
    }

    unsigned char bin_integral_part[2] = {data[n_bytes-4], data[n_bytes-3]};
    unsigned char bin_fractional_part[2] = {data[n_bytes-2], data[n_bytes-1]};

    int integral_part(readInt32(bin_integral_part,2));
    float fractional_part(readInt32(bin_fractional_part,2));

    while(fractional_part > 2)
        fractional_part /= 10;

    fractional_part -= 1.0f;

    return (integral_part+fractional_part);
}

/**************
 * FILE UTILS *
 **************/
bool FileUtils::init_directory_structure(QString directory, QString & radial_distribution_folder, QString & category_properties_folder,
                                        QString & csv_files_folder)
{
    if(!directory.endsWith("/"))
        directory.append("/");
    // Transform to OS directory format
    directory = QDir::toNativeSeparators(directory);

    if(!QDir(directory).exists())
    {
        std::cerr << "Folder: " << directory.toStdString() << " doesn't exist!" << std::endl;
        return false;
    }

    // Init radial distribution subdirectory
    {
        radial_distribution_folder = directory;
        radial_distribution_folder.append(RADIAL_DISTRIBUTION_SUBFOLDER).append(QDir::separator());
        QDir radial_distribution_dir( radial_distribution_folder );
        if(!radial_distribution_dir.exists() && !radial_distribution_dir.mkpath("."))
            return false;
    }
    // Init category properties subdirectory
    {
        category_properties_folder = directory;
        category_properties_folder.append(CATEGORY_PROPERTIES_SUBFOLDER).append(QDir::separator());
        QDir category_properties_dir( category_properties_folder );
        if(!category_properties_dir.exists() && !category_properties_dir.mkpath("."))
            return false;
    }
    // Init csv descriptors subdirectory
    {
        csv_files_folder = directory;
        csv_files_folder.append(CSV_FILES_SUBFOLDER).append(QDir::separator());
        QDir csv_dir( csv_files_folder );
        if(!csv_dir.exists() && !csv_dir.mkpath("."))
            return false;
    }

    return true;
}

bool FileUtils::check_directory_structure(QString directory)
{
    if(!directory.endsWith("/"))
        directory.append("/");
    // Transform to OS directory format
    directory = QDir::toNativeSeparators(directory);

    // Ensure directory exists
    if(!QDir(directory).exists())
    {
        std::cout << "Directory: " << directory.toStdString() << " doesn't exist!" << std::endl;
        return false;
    }

    // Check radial distribution subdirectory exists
    {
        QString radial_distribution_folder(directory);
        radial_distribution_folder.append(RADIAL_DISTRIBUTION_SUBFOLDER);
        if(!QDir(radial_distribution_folder).exists())
        {
            std::cout << "Directory: " << directory.toStdString() << " doesn't exist!" << std::endl;
            return false;
        }
    }

    // Check size properties subdirectory exists
    {
        QString category_properties_folder(directory);
        category_properties_folder.append(CATEGORY_PROPERTIES_SUBFOLDER);
        if(!QDir(category_properties_folder).exists())
        {
            std::cout << "Directory: " << directory.toStdString() << " doesn't exist!" << std::endl;
            return false;
        }
    }

    // Ensure the configuration file exists
    {
        QString configuration_file(directory);
        configuration_file.append(CONFIGURATION_FILE_NAME);
        if(!QFile(configuration_file).exists())
        {
            std::cout << "Configuration file: " << configuration_file.toStdString() << " doesn't exist!" << std::endl;
            return false;
        }
    }

    return true;
}

std::vector<QString> FileUtils::get_radial_distribution_files(QString directory)
{
    if(!directory.endsWith("/"))
        directory.append("/");
    // Transform to OS directory format
    directory.append(RADIAL_DISTRIBUTION_SUBFOLDER);
    directory = QDir::toNativeSeparators(directory);

    QDir dir(directory);
    QStringList name_filter("*");
    name_filter.append(RADIAL_DISTRIBUTION_FILE_EXTENSION);

    std::vector<QString> ret;
    for(QString radial_distribution_file : dir.entryList(name_filter, QDir::Files))
    {
        QString full_path(directory);
        full_path.append(QDir::separator()).append(radial_distribution_file);
        ret.push_back(full_path);
    }

    return ret;
}

std::vector<QString> FileUtils::get_category_properties_files(QString directory)
{
    if(!directory.endsWith("/"))
        directory.append("/");
    // Transform to OS directory format
    directory.append(CATEGORY_PROPERTIES_SUBFOLDER);
    directory = QDir::toNativeSeparators(directory);

    QDir dir(directory);
    QStringList name_filter("*");
    name_filter.append(CATEGORY_PROPERTIES_FILE_EXTENSION);

    std::vector<QString> ret;
    for(QString category_file : dir.entryList(name_filter, QDir::Files))
    {
        QString full_path(directory);
        full_path.append(QDir::separator()).append(category_file);
        ret.push_back(full_path);
    }

    return ret;
}

QString FileUtils::get_configuration_file(QString directory)
{
    if(!directory.endsWith("/"))
        directory.append("/");
    // Transform to OS directory format
    directory = QDir::toNativeSeparators(directory);

    return directory.append(CONFIGURATION_FILE_NAME);
}
