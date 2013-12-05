#ifndef ELEVATIONMODEL_H
#define ELEVATIONMODEL_H

#include <QString>
#include <QSize>

#define HGT_RANGE 65536

class HGTFormatException
{
    QString error;
public:
    QString getError(){return error;}
    HGTFormatException(QString s):error(s){}
};

class ElevationModel
{
public:
    ElevationModel(QString &filename);
    unsigned short at(int x, int y) const;
    QSize size() const;
    unsigned short getMin() const;
    unsigned short getMax() const;
    float getAverage() const;
    float getDeviation() const;
private:
    unsigned short *data;
    unsigned short res;
    unsigned short min, max;
    float average, deviation;
};

#endif // ELEVATIONMODEL_H
