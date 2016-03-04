//
//
//  Distrib - a class encoding an ecosystem distribution over an entire terrain, including file I/O
//
//  Created by James Gain on 2016/01/31.

#ifndef distrib_h
#define distrib_h

#include <vector>
#include <stdio.h>
#include <string>
#include "vecpnt.h"

using namespace std;

class Distrib
{
public:

    struct PlantInstance
    {
        int height;
        cgp::Point pos;
    };

    struct SpeciesProperties
    {
        string speciesName;
        int minHeight;
        int maxHeight;
        float heightToCanopyRadius;
        vector<PlantInstance> instances;
    };

    /**
     * Print out the ecosystem contents to standard out for debugging purposes
     */
    void print();
    
    /**
     * Read in plant positions from a PDB format text file
     * @param filename  name of file to load (PDB format)
     * @retval true  if load succeeds,
     * @retval false otherwise.
     */
    bool readPDB(string filename);

    /**
     * Write plant positions out to a PDB format text file
     * @param filename  name of file to save (STL format)
     * @retval true  if save succeeds,
     * @retval false otherwise.
     */
    bool writePDB(string filename);

    void setEcosystem(vector<SpeciesProperties> inEcoSys); // Sets the ecosystem

private:

    vector<SpeciesProperties> ecoSys;
};


#endif /* distrib_h */
