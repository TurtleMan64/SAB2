#ifndef RHRAMP_H
#define RHRAMP_H

class TexturedModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class RH_Ramp : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    RH_Ramp();
    RH_Ramp(float x, float y, float z, float rotY, float rotZ);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
