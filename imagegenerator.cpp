#include "imagegenerator.h"
#include <QImage>
#include "elevationmodel.h"
#include "gradient.h"


QImage *ImageGenerator::generate(ElevationModel *em, Gradient *g)
{
    QImage *output = new QImage(em->size(), QImage::Format_RGB32);
    for (int i=0; i<output->width(); i++)
        for (int j=0; j<output->height(); j++)
            output->setPixel(i,output->height()-j-1,g->getColorF(float(em->at(i,j))/HGT_RANGE).rgb());
    return output;
}
