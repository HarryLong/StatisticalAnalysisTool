#ifndef POINT_PROPERTIES_H
#define POINT_PROPERTIES_H

#include <map>
#include <string>

#define SIGNATURE "POINTPROPERTIESFILE"
#define SIGNATURE_LENGTH 19

struct PointSizePropertiesHeader{
public:
    PointSizePropertiesHeader() {}
    PointSizePropertiesHeader(int points_id, int n_points, int min, int max, int diff) :
        points_id(points_id), n_points(n_points), min(min), max(max), diff(diff)
    {}

    int points_id;
    int min;
    int max;
    int diff;
    int n_points;
};

class PointSizeProperties
{
public:
    typedef std::map<int,float> Histogram;

    PointSizeProperties(PointSizePropertiesHeader header, Histogram data);
    PointSizeProperties(std::string filename);
    void write(std::string filename);
    void writeToCSV(std::string filename);

    PointSizePropertiesHeader m_header;
    Histogram m_data;
private:
    bool load_data(std::string filename);
};

#endif // POINT_PROPERTIES_H
