#ifndef REPRODUCTION_CONFIGURATION_H
#define REPRODUCTION_CONFIGURATION_H

#include <QString>

class ReproductionConfiguration{
public:
    enum InitializationType{
        TwoPoints,
        MatchDensity
    };

    enum GenerationAlgorithm{
        BirthsAndDeaths,
        RandomMoves
    };

    const int width, height, n_iterations;
    const InitializationType initialization_type;
    const GenerationAlgorithm generation_algo;
    const QString active_directory;

    ReproductionConfiguration(QString active_directory, int width, int height, int n_iterations, InitializationType init_type = InitializationType::MatchDensity,
             GenerationAlgorithm generation_algo = GenerationAlgorithm::RandomMoves);
};

#endif //REPRODUCTION_CONFIGURATION_H
