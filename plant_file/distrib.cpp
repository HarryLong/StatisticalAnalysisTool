//
//
//  Distrib - a class encoding an ecosystem distribution over an entire terrain, including file I/O
//
//  Created by James Gain on 2016/01/31.

#include "distrib.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void Distrib::print()
{
    int numSpecies, numPlants;
    cgp::Point loc;

    numSpecies = (int) ecoSys.size();
    cout << "numSpecies = " << numSpecies << endl;

    for(int i = 0; i < numSpecies; i++)
    {
        cout << "speciesName = " << ecoSys[i].speciesName << endl;
        cout << "minHeight = " << ecoSys[i].minHeight << " maxHeight = " << ecoSys[i].maxHeight << " radiusMultiplier: " << ecoSys[i].heightToCanopyRadius << endl;
        numPlants = (int) ecoSys[i].instances.size();
        cout << "numPlants = " << numPlants << endl;
        for(int j = 0; j < numPlants; j++)
        {
            loc = ecoSys[i].instances[j].pos;
            cout << loc.x << " " << loc.y << " " << loc.z << endl;
        }
    }
}

bool Distrib::readPDB(string filename)
{
    ifstream infile;
    int numSpecies, numPlants;

    // assumes text format PDB file
    // number of species (n)
    // followed by n species in sequence
    //


    infile.open((char *) filename.c_str(), ios_base::in);
    if(infile.is_open())
    {
        ecoSys.clear(); // new ecosystem overwrites old

        infile >> numSpecies;
        for(int i = 0; i < numSpecies; i++)
        {
            SpeciesProperties sdist;
            infile >> sdist.speciesName;
            infile >> sdist.minHeight >> sdist.maxHeight >> sdist.heightToCanopyRadius;
            infile >> numPlants;
            for(int j = 0; j < numPlants; j++)
            {
                PlantInstance instance;
                infile >> instance.pos.x >> instance.pos.y >> instance.pos.z >> instance.height;
                sdist.instances.push_back(instance);
            }
            ecoSys.push_back(sdist);
        }
        infile.close();
    }
    else
    {
        cerr << "Error Mesh::readPDB: unable to open " << filename << endl;
        return false;
    }
    return true;

}

bool Distrib::writePDB(string filename)
{
    ofstream outfile;
    int numSpecies, numPlants;
    cgp::Point instance_pos;

    outfile.open((char *) filename.c_str(), ios_base::out);
    if(outfile.is_open())
    {
        numSpecies = (int) ecoSys.size();
        outfile << numSpecies << endl;

        for(int i = 0; i < numSpecies; i++)
        {
            outfile << ecoSys[i].speciesName << endl;
            outfile << ecoSys[i].minHeight << " " << ecoSys[i].maxHeight << " " << ecoSys[i].heightToCanopyRadius << endl;
            numPlants = (int) ecoSys[i].instances.size();
            outfile << numPlants << endl;
            for(int j = 0; j < numPlants; j++)
            {
                instance_pos = ecoSys[i].instances[j].pos;
                outfile << instance_pos.x << " " << instance_pos.y << " " << instance_pos.z << " " << ecoSys[i].instances[j].height << endl;
            }
        }

        // tidy up
        outfile.close();
    }
    else
    {
        cerr << "Error Mesh::writeSTL: unable to open " << filename << endl;
        return false;
    }
    return true;
}


void Distrib::setEcosystem(vector<SpeciesProperties> inEcoSys) // Sets the ecosystem
{
    ecoSys = inEcoSys;
}
