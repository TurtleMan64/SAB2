#ifndef CAMERA_H
#define CAMERA_H

#include "../toolbox/vector.h"

class Camera
{
public:
    Vector3f fadePosition1;
    Vector3f fadePosition2;

    Vector3f eye;
    Vector3f target;
    Vector3f up;

    Vector3f eyePrevious;
    Vector3f vel;

    bool inWater = false;

public:
    Camera();

    //Call this ONCE per frame, it calculates velocity and other things
    void refresh();

    //changes the camera so that it becomes a mirror image of itself, with
    //the mirror plane being the water
    void mirrorForWater();

    Vector3f* getFadePosition1();

    Vector3f* getFadePosition2();

    void setViewMatrixValues(Vector3f* newEye, Vector3f* newTarget, Vector3f* newUp);
};
#endif