#ifndef NBWATERPLATFORM_H
#define NBWATERPLATFORM_H

class TexturedModel;

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class NB_WaterPlatform : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    NB_WaterPlatform();
    NB_WaterPlatform(float x, float y, float z, float rotY);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
