#ifndef LIGHT_H
#define LIGHT_H

#include "../toolbox/vector.h"

class Light
{
public:
    Vector3f position;
    Vector3f direction;
    Vector3f color;

    Light();
};
#endif
