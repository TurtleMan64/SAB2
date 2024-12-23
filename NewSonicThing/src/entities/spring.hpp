#ifndef SRING_H
#define SRING_H

class TexturedModel;

#include <list>
#include <vector>
#include "entity.hpp"


class Spring : public Entity
{
public:
    static std::vector<TexturedModel*> models;

    float springPower;
    float cooldownTimer;
    float cooldownTimerMax;
    bool resetsCamera; //when you hit the spring, does the camera auto adjust?

    Vector3f dir; //direction that the spring is facing
    Vector3f hitCenter; //the center point of hit detection for the spring

    Spring();
    Spring(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float cooldownMax, bool setsCam);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;
};
#endif
