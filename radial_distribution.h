#ifndef RADIAL_DISTRIBUTION_H
#define RADIAL_DISTRIBUTION_H

#include <map>
#include <string>

#define SIGNATURE "RADIALDISTRIBUTIONFILE"
#define SIGNATURE_LENGTH 22

struct RadialDistributionPorperties{
    unsigned int reference_id;
    unsigned int destination_id;
    unsigned int n_reference_points;
    unsigned int n_destination_points;
    unsigned int analysed_area;
    unsigned int r_min;
    unsigned int r_max;
    unsigned int r_diff;

    RadialDistributionPorperties(unsigned int reference_id,
                                 unsigned int destination_id,
                                 unsigned int n_reference_points,
                                 unsigned int n_destination_points,
                                 unsigned int analysed_area,
                                 unsigned int r_min,
                                 unsigned int r_max,
                                 unsigned int r_diff) :
        reference_id(reference_id),
        destination_id(destination_id),
        n_reference_points(n_reference_points),
        n_destination_points(n_destination_points),
        analysed_area(analysed_area),
        r_min(r_min),
        r_max(r_max),
        r_diff(r_diff){}

    RadialDistributionPorperties() {}
};

class RadialDistribution{
public:
    typedef std::map<int,float> Histogram;

    RadialDistribution(RadialDistributionPorperties properties, Histogram data);
    RadialDistribution(std::string filename);

    void write(std::string filename);

    void printToConsole();
    void writeToCSV(std::string filename);

    Histogram m_data;
    RadialDistributionPorperties m_properties;
private:
    bool load(std::string filename);
};

#endif //RADIAL_DISTRIBUTION_H
