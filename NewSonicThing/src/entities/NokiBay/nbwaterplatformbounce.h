#ifndef NBWATERPLATFORMBOUNCE_H
#define NBWATERPLATFORMBOUNCE_H

class TexturedModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class NB_WaterPlatformBounce : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    NB_WaterPlatformBounce();
    NB_WaterPlatformBounce(float x, float y, float z, float rotY);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
