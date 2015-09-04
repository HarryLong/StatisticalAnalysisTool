#include "reproduction_configuration.h"

ReproductionConfiguration::ReproductionConfiguration(QString active_directory, int width, int height, int n_iterations,
                                                     InitializationType init_type, GenerationAlgorithm generation_algo):
    active_directory(active_directory), width(width), height(height), n_iterations(n_iterations), initialization_type(init_type), generation_algo(generation_algo)
{

}
