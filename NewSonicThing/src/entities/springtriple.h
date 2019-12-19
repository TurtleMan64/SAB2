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

    Vector3f hitCenter1; //the center point of left spring
    Vector3f hitCenter2; //the center point of middle spring
    Vector3f hitCenter3; //the center point of right spring

    Vector3f end1; //endpoint of one side of the set of springs
    Vector3f end2; //endpoint of the other side of the set of springs

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
