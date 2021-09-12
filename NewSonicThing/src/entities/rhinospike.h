#ifndef RHINOSPIKE_H
#define RHINOSPIKE_H

class TexturedModel;
class Triangle3D;

#include <list>
#include "entity.h"


class RhinoSpike : public Entity
{
private:
    static std::list<TexturedModel*> models;

    const float activationRange = 64.0f; //range where they can see sonic
    const float moveRange = 500.0f; //range where they can see sonic
    const float lookSpeed = 6.0f; //how fast they turn towards sonic

    const float hitboxRadius = 16.89f;
    const float hitboxHeight = 10.36f;
    const float hurtboxRadius = 11.78f;
    const float hurtboxHeight = 14.55f;

    Vector3f lookDir;

    Triangle3D* triCol = nullptr;
    float xVel = 0;
    float yVel = 0;
    float zVel = 0;
    float furyTimer = 0;
    const float furyTimerMax = 2.5f;
    bool inAir = true;
    float clock = 0;

    void applyFriction(float frictionToApply);

    void moveMeSlow();

    void moveMe();

    bool playerIsInHurtbox();

    bool playerIsInHitbox();

public:
    RhinoSpike();
    RhinoSpike(float x, float y, float z);

    void step();

    void die();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;

    bool isEnemy() const;
};
#endif
