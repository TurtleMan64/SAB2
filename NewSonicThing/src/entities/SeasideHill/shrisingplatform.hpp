#ifndef SH_RISINGPLATFORM_H
#define SH_RISINGPLATFORM_H

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;

class SH_RisingPlatform : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

    int type = 0;
    float movingDistance = 1.0f;
    float originalY = 0.0f;
    float triggerTimer = 0.0f;
    float triggerX = 0.0f;
    float triggerZ = 0.0f;
    float triggerR = 1.0f;

public:
    SH_RisingPlatform(float x, float y, float z, int type, float distance, float tX, float tZ, float tR);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
