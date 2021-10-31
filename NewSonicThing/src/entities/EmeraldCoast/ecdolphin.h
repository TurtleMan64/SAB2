#ifndef ECDOLPHIN_H
#define ECDOLPHIN_H

class TexturedModel;

#include <list>
#include "../entity.h"

class EC_Dolphin : public Entity
{
private:
    static std::list<TexturedModel*> models;

    float triggerX = 0.0f;
    float triggerY = 0.0f;
    float triggerZ = 0.0f;

    float triggerRadius = 0.0f;

    float triggerTimer = 0.0f;

    float yVel = 0.0f;
    float hSpd = 0.0f;

    bool inWaterPrevious = true;

public:
    EC_Dolphin();
    EC_Dolphin(
        float x, float y, float z,
        float dx, float dz,
        float triggerX, float triggerY, float triggerZ,
        float triggerRadius);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
