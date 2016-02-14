#ifndef REPRODUCTION_CONFIGURATION_H
#define REPRODUCTION_CONFIGURATION_H

#include <QString>

class ReproductionConfiguration{
public:
    const int width, height, n_iterations;
    const QString active_directory;

    ReproductionConfiguration(QString active_directory, int width, int height, int n_iterations);
};

#endif //REPRODUCTION_CONFIGURATION_H
