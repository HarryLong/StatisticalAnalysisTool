#include "dice_roller.h"

DiceRoller::DiceRoller(int from, int to)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = new std::default_random_engine(seed);
    distribution = new std::uniform_int_distribution<int>(from,to);
}

DiceRoller::~DiceRoller()
{
    delete generator;
    delete distribution;
}

int DiceRoller::generate()
{
    return distribution->operator()(*generator);
}
