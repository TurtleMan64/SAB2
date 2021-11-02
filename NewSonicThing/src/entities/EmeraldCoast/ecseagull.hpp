#ifndef ECSEAGULL_H
#define ECSEAGULL_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class EC_Seagull : public Entity
{
private:
    static std::list<TexturedModel*> models;

    float modelX = 0.0f;
    float modelY = 0.0f;
    float modelZ = 0.0f;

    float prevX = 0.0f;
    float prevY = 0.0f;
    float prevZ = 0.0f;

    float flyAngle = 0.0f;
    float flyAngleDelta = 0.0f;

    float flyRadiusBase = 0.0f;
    float flyRadiusAmplitude = 0.0f;
    float flyRadiusDelta = 0.0f;
    float flyRadiusProgression = 0.0f;

    float flyHeightBase = 0.0f;
    float flyHeightAmplitude = 0.0f;
    float flyHeightDelta = 0.0f;
    float flyHeightProgression = 0.0f;

    float soundTimer = 1.0f;

public:
    EC_Seagull();
    EC_Seagull(float x, float y, float z);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
