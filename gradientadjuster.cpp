#include "gradientadjuster.h"
#include "gradient.h"
#include "elevationmodel.h"
#include <QVector>
#include "stop.h"
#include "logger.h"

Gradient *GradientAdjuster::getAdjusted(Gradient *gradient, ElevationModel *em)
{
    float min = (em->getAverage()-1.5*em->getDeviation())/HGT_RANGE;
    float newRange = (3*em->getDeviation())/HGT_RANGE;

    Gradient *output = new Gradient(*gradient);
    QVector<Stop*> stops = output->getStops();
    for (int i=0; i<stops.size(); i++)
        stops.at(i)->setPosition(stops.at(i)->getPosition()*newRange + min*100000);
    return output;
}
