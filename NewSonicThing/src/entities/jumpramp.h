#ifndef JUMPRAMP_H
#define JUMPRAMP_H

class TexturedModel;

#include <list>
#include "entity.h"

class JumpRamp : public Entity
{
private:
    static std::list<TexturedModel*> models;

    bool playerIsIn;

public:
    JumpRamp();
    JumpRamp(float x, float y, float z, float yRot);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif