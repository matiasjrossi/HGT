#ifndef GRADIENTADJUSTER_H
#define GRADIENTADJUSTER_H

class Gradient;
class ElevationModel;

class GradientAdjuster
{
public:
    static Gradient *getAdjusted(Gradient *gradient, ElevationModel *em);
};

#endif // GRADIENTADJUSTER_H
