#ifndef GUMDROP_H
#define GUMDROP_H

class TexturedModel;

#include <vector>
#include "../entity.hpp"
#include "../../toolbox/vector.hpp"

class Gumdrop : public Entity
{
public:
    static std::vector<TexturedModel*> models;

    float springPower;
    float cooldownTimer = 0.0f;
    const float cooldownTimerMax = 0.5f;

    Vector3f dir; //direction that the spring is facing
    Vector3f hitCenter; //the center point of hit detection for the spring

    Gumdrop();
    Gumdrop(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;
};
#endif
