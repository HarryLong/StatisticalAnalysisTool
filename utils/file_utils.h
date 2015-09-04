#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <QString>
#include <vector>


class FileUtils
{
public:
    static const char * _CONFIGURATION_FILENAME;
    static const char * _RADIAL_DISTRIBUTION_EXT;
    static const char * _CATEGORY_PROPERTIES_EXT;

    static unsigned char* toBin(unsigned int value, int n_bytes = 4);
    static unsigned char* toBin(float value, int n_bytes = 4);

    static int readInt32(unsigned char * data, int n_bytes = 4);
    static float readFloat32(unsigned char * data, int n_bytes = 4);

    static void insertPadding(unsigned char * data, int from, int to);

    static bool init_directory_structure(QString directory, QString & radial_distribution_folder, QString & category_properties_folder,
                              QString & csv_files_folder);
    static bool check_directory_structure(QString directory);
    static std::vector<QString> get_radial_distribution_files(QString directory);
    static std::vector<QString> get_category_properties_files(QString directory);
    static QString get_configuration_file(QString directory);
};


#endif // FILE_UTILS_H
