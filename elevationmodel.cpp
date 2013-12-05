#include "elevationmodel.h"
#include "logger.h"
#include <math.h>

#define M01_SIZE 720000
#define M01_RES 600
#define ONE_ARC_SEC_SIZE 25934402
#define ONE_ARC_SEC_RES 3601
#define THREE_ARC_SEC_SIZE 2884802
#define THREE_ARC_SEC_RES 1201

ElevationModel::ElevationModel(QString &filename) :
        data(NULL),
        res(0),
        min(HGT_RANGE - 1),
        max(0),
        average(0.0),
        deviation(0.0)
{
    //Try to open file
    FILE * file = fopen(filename.toStdString().c_str(),"rb");
    if(file==NULL)
        throw HGTFormatException("Couldn't open file: " + filename + ".\n¿Do you have read permissions?");

    //Check file size
    fseek (file , 0 , SEEK_END);
    switch(ftell (file))
    {
    case M01_SIZE:
        res = M01_RES;
        break;
    case ONE_ARC_SEC_SIZE:
        res = ONE_ARC_SEC_RES;
        break;
    case THREE_ARC_SEC_SIZE:
        res = THREE_ARC_SEC_RES;
        break;
    default:
        fclose(file);
        throw HGTFormatException("Invalid file size.");
    }
    rewind(file);
    data = new unsigned short[res*res];

    unsigned char b,a;
    //Data reading
    long samples = res*res;
    for(unsigned x=0;x<res;x++){
        for(unsigned y=0;y<res;y++){
            //16-bit signed in meters above sea level
            fread(&a,1,1,file);
            fread(&b,1,1,file);
            unsigned short val;
            if (res == M01_RES)
                val = (((short)a)*256 + b);
            else
                val = (((short)a)*256 + b) + 32768;
            data [x*res+y] = val;
            if (val != 0)
            {
                min = (val < min ? val : min);
                max = (val > max ? val : max);
            }
            average += float(val)/samples;
        }
    }
    for (unsigned x=0; x<res; x++){
        for (unsigned y=0; y<res; y++)
        {
            deviation += pow(data [x*res+y] - average, 2)/samples;
        }
    }
    deviation = sqrt(deviation);
    Logger::get()->debug(QString("ElevationModel::ElevationModel(QString &filename)  [minimum: %1; maximum:%2; average:%3; std. deviation:%4]").arg(min).arg(max).arg(average).arg(deviation));
    fclose (file);
}

unsigned short ElevationModel::at(int x, int y) const
{
    if (res == 0 || data == NULL)
        Logger::get()->fatal("ElevationModel::at [Uninitialized Elevation Model]");
    return data[x*res+y];
}

QSize ElevationModel::size() const
{
    return QSize(res,res);
}

unsigned short ElevationModel::getMin() const
{
    return this->min;
}

unsigned short ElevationModel::getMax() const
{
    return this->max;
}

float ElevationModel::getAverage() const
{
    return this->average;
}

float ElevationModel::getDeviation() const
{
    return this->deviation;
}
