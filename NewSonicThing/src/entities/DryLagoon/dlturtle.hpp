#ifndef DLTURTLE_H
#define DLTURTLE_H

class TexturedModel;

#include <list>

#include "../../toolbox/vector.hpp"
#include "../entity.hpp"
#include "../collideableobject.hpp"

class DL_Turtle : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static CollisionModel* cmOriginal;

    //target values
    Vector3f target;
    Vector3f targetCam;

    float warpTimer = 0.0f;
    const float warpTimerMax = 1.0f;

public:
    DL_Turtle();
    DL_Turtle(float x, float y, float z, float rotY,
              float xt, float yt, float zt,
              float camX, float camY, float camZ);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
