#ifndef RADIAL_DISTRIBUTION_H
#define RADIAL_DISTRIBUTION_H

#include <map>
#include <string>
#include <memory>

struct RadialDistributionHeader{

    static std::string _SIGNATURE;

    unsigned int reference_id;
    unsigned int destination_id;
    bool requires_optimization;

    RadialDistributionHeader(unsigned int reference_id,
                             unsigned int destination_id,
                             bool requires_optimization) :
        reference_id(reference_id),
        destination_id(destination_id),
        requires_optimization(requires_optimization){}

    RadialDistributionHeader() {}

    bool operator ==(const RadialDistributionHeader &other) const
    {
        return (reference_id == other.reference_id && destination_id == other.destination_id &&
                requires_optimization == other.requires_optimization);
    }
};

class RadialDistribution{
public:
    typedef std::map<int,float> Histogram;

    RadialDistribution(RadialDistributionHeader properties, float within_radius_distribution, float past_rmax_distribution, Histogram data);
    RadialDistribution(std::string filename);
    RadialDistribution();

    void write(std::string filename) const;

    void printToConsole() const;
    void writeToCSV(std::string filename) const;
    float getMaximum() const;
    float getMinimum() const;

    float m_within_radius_distribution;
    float m_past_rmax_distribution;
    Histogram m_data;
    RadialDistributionHeader m_header;
private:
    bool load(std::string filename);
    void calculate_min_max();
    float m_maximum;
    float m_minimum;
};

#endif //RADIAL_DISTRIBUTION_H
