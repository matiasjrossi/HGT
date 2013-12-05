#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

class QImage;
class ElevationModel;
class Gradient;

class ImageGenerator
{
public:
    static QImage *generate(ElevationModel *, Gradient *);
};

#endif // IMAGEGENERATOR_H
