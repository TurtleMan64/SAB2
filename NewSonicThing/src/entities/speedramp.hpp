#ifndef SPEEDRAMP_H
#define SPEEDRAMP_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "collideableobject.hpp"

class SpeedRamp : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static CollisionModel* cmOriginal;

    Vector3f launchDirection;
    float power;
    float inputLockDuration;

public:
    SpeedRamp();
    SpeedRamp(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float inputLockDuration);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
