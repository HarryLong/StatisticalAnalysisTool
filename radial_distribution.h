#ifndef RADIAL_DISTRIBUTION_H
#define RADIAL_DISTRIBUTION_H

#include <map>
#include <string>
#include <memory>

#define RADIAL_DISTRIBUTION_SIGNATURE "RADIALDISTRIBUTIONFILE"
#define RADIAL_DISTRIBUTION_SIGNATURE_LENGTH 22
struct RadialDistributionHeader{
    unsigned int reference_id;
    unsigned int destination_id;

    RadialDistributionHeader(unsigned int reference_id,
                             unsigned int destination_id) :
        reference_id(reference_id),
        destination_id(destination_id) {}

    RadialDistributionHeader() {}

    bool operator ==(const RadialDistributionHeader &other) const
    {
        return (reference_id == other.reference_id && destination_id == other.destination_id);
    }
};

class RadialDistribution{
public:
    typedef std::map<int,float> Histogram;

    RadialDistribution(RadialDistributionHeader properties, float within_radius_distribution, Histogram data);
    RadialDistribution(std::string filename);
    RadialDistribution();

    void write(std::string filename) const;

    void printToConsole() const;
    void writeToCSV(std::string filename) const;
    float getMaximum();

    float m_within_radius_distribution;
    Histogram m_data;
    RadialDistributionHeader m_header;
private:
    bool load(std::string filename);
    float m_maximum;
};

#endif //RADIAL_DISTRIBUTION_H
