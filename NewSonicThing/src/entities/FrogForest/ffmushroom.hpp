#ifndef FF_MUSHROOM_H
#define FF_MUSHROOM_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class FF_Mushroom : public Entity
{
public:
    static std::vector<TexturedModel*> models;

    static constexpr float springPower = 400.0f;
    static constexpr float springRadius = 20.4f;
    float cooldownTimer = 0.0f;
    static constexpr float cooldownTimerMax = 0.2f;

    Vector3f hitCenter; //the center point of hit detection for the spring

    FF_Mushroom();
    FF_Mushroom(float x, float y, float z, float yRot);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;
};
#endif
