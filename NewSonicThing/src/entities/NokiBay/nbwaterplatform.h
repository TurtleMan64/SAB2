#ifndef NBWATERPLATFORM_H
#define NBWATERPLATFORM_H

class TexturedModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class NB_WaterPlatform : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    NB_WaterPlatform();
    NB_WaterPlatform(float x, float y, float z, float rotY);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
