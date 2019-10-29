#ifndef SRINGTRIPLE_H
#define SRINGTRIPLE_H

class TexturedModel;

#include <list>
#include <vector>
#include "entity.h"


class SpringTriple : public Entity
{
public:
    static std::list<TexturedModel*> models;

    float springPower;
    float cooldownTimer;
    float cooldownTimerMax;

    Vector3f dir; //direction that the spring is facing
    Vector3f hitCenter; //the center point of hit detection for the spring

    SpringTriple();
    SpringTriple(float x, float y, float z, float dirX, float dirZ, float myPower, float cooldownMax);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    const bool canHomingAttackOn();

    const Vector3f getHomingCenter();
};
#endif
