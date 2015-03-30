#include "utils.h"
#include <malloc.h>
#include <algorithm>
#include <iostream>
#include <QImage>

#define MAX_8_BITS 256
#define MAX_16_BITS 65536

/*************
 * BIN UTILS *
 *************/
void Binutils::insertPadding(unsigned char * data, int from, int to)
{
    for(int i(from); i < to; i++)
        data[i] = 0x00;
}

unsigned char* Binutils::toBin(unsigned int value, int n_bytes)
{
    unsigned char* ret = new unsigned char[n_bytes];
    int byte_index(0);
    // First the padding
    if(n_bytes > 4)
    {
        insertPadding(ret, 0, n_bytes-4);
        byte_index += (n_bytes-4);
    }

    // Now the data
    int remaining_bytes(std::min(n_bytes, 4));
    for(int i(0); i < remaining_bytes; i++)
    {
        unsigned char data ( (value >> ((remaining_bytes-1-i)*8)) & int(0x00000FF));
        ret[byte_index++] = data;
    }

    return ret;
}

/**
  Very simple modeling of floating point values where the first 16 bits represent the integral part and the next 16 bits represent
  the fractional part.
 * @brief Binutils::toBin
 * @param value
 * @param n_bytes
 * @return
 */
unsigned char* Binutils::toBin(float value, int n_bytes)
{
    if(n_bytes < 4)
    {
        std::cerr << "Can't represent a float in " << n_bytes << " bytes";
        exit(1);
    }

    unsigned char* ret = new unsigned char[n_bytes];
    int byte_index(0);

    // first the padding
    // First the padding
    if(n_bytes > 4)
    {
        insertPadding(ret, 0, n_bytes-4);
        byte_index += (n_bytes-4);
    }

    // Now the data
    unsigned int integral_part(value);
    unsigned int fractional_part;
    {
        float tmp_fractional_part(value-integral_part + 1.0f);

        while((tmp_fractional_part*10) < MAX_16_BITS)
            tmp_fractional_part *= 10;
        fractional_part = (unsigned int) tmp_fractional_part;
    }

    unsigned char * bin_integral_part (toBin(integral_part,2)) ;
    unsigned char * bin_fractional_part (toBin(fractional_part, 2));

//    std::cout << "integral_part " << integral_part << std::endl;
//    std::cout << "fractional " << fractional_part << std::endl;

    ret[byte_index++] = bin_integral_part[0];
    ret[byte_index++] = bin_integral_part[1];
    ret[byte_index++] = bin_fractional_part[0];
    ret[byte_index++] = bin_fractional_part[1];

    delete [] bin_integral_part;
    delete [] bin_fractional_part;

    return ret;
}

int Binutils::readInt32(unsigned char * data, int n_bytes)
{
    int value(0);

    int padding_bytes(std::max(0,n_bytes-4));

    int remaining_bytes(n_bytes-padding_bytes);
    int coefficient((remaining_bytes-1)*8);
    for(int i(0); i < remaining_bytes; i++,coefficient -= 8)
    {
        value += (data[i+padding_bytes] * pow(2, coefficient));
    }

    return value;
}

float Binutils::readFloat32(unsigned char * data, int n_bytes)
{
    if(n_bytes < 4)
    {
        std::cerr << "Can't represent a float in " << n_bytes << " bytes";
        exit(1);
    }

    unsigned char bin_integral_part[2] = {data[n_bytes-4], data[n_bytes-3]};
    unsigned char bin_fractional_part[2] = {data[n_bytes-2], data[n_bytes-1]};

    int integral_part(readInt32(bin_integral_part,2));
    float fractional_part(readInt32(bin_fractional_part,2));

    while(fractional_part > 2)
        fractional_part /= 10;

    fractional_part -= 1.0f;

    return (integral_part+fractional_part);
}

/*****************************
 * RADIAL DISTRIBUTION UTILS *
 *****************************/
int RadialDistributionUtils::getRBracket(double distance, int r_min, int r_diff)
{
    return r_min + ((int)((distance-r_min)/r_diff)) * r_diff;
}

/***********************
 * PROBABILISTIC UTILS *
 ***********************/
// DiceRoller must be set to return a value between 0 and 1000
bool ProbabilisticUtils::returnTrueWithProbability(float probability, DiceRoller & dice_roller)
{
    int i_probability(probability*1000);

    return (dice_roller.generate() < i_probability);
}

/**************
 * FILE UTILS *
 **************/
void FileUtils::printPointsToImg(std::string image_file, const std::vector<QPoint> & points, int width, int height)
{
    QImage output_img(width, height, QImage::Format_RGB32);
    output_img.fill(qRgb(0,0,0));

    for(QPoint p : points)
        output_img.setPixel(p.x(), p.y(), qRgb(255,255,255));
    output_img.save(QString(image_file.c_str()));
}

