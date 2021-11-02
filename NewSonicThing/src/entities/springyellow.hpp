#ifndef SPRINGYELLOW_H
#define SPRINGYELLOW_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class SpringYellow : public Entity
{
public:
    static std::list<TexturedModel*> models;

    float springPower = 500.0f;
    float springRadius = 5.0f;
    float cooldownTimer = 0.0f;
    float cooldownTimerMax = 0.0f;

    float fastCheckRadius = 0.0f;
    Vector3f collisionP1;
    Vector3f collisionP2;

    Vector3f dir; //direction that the spring is facing
    Vector3f hitCenter; //the center point of hit detection for the spring

    SpringYellow();
    SpringYellow(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float cooldownMax, float radius);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;
};
#endif
