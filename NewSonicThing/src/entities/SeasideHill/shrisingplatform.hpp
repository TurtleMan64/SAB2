#ifndef SH_RISINGPLATFORM_H
#define SH_RISINGPLATFORM_H

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;

class SH_RisingPlatform : public CollideableObject
{
private:
    static std::vector<TexturedModel*> modelsSmall;
    static std::vector<TexturedModel*> modelsMedium;
    static std::vector<TexturedModel*> modelsLarge;
    static CollisionModel* cmSmallOriginal;
    static CollisionModel* cmMediumOriginal;
    static CollisionModel* cmLargeOriginal;

    int type = 0;
    float finalY = 1.0f;
    float originalY = 0.0f;
    float triggerTimer = 0.0f;
    float triggerX = 0.0f;
    float triggerZ = 0.0f;
    float triggerR = 1.0f;

public:
    SH_RisingPlatform(float x, float y, float z, float rotY, int type, float finalY, float tX, float tZ, float tR);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
