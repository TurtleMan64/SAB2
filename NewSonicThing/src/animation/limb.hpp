#ifndef LIMB_H
#define LIMB_H

class Vector3f;
class TexturedModel;
class Body;

#include <list>
#include "../entities/entity.hpp"
#include "animation.hpp"


class Limb : public Entity
{
private:
    Body* body = nullptr;
    Limb* limb = nullptr;

    //We do not store our own models. It is up to 
    // whoever created us to create the models and keep
    // them allocated as long as we are alive.
    std::vector<TexturedModel*>* myModels;

public:
    int animationIndex;
    float pivotX;
    float pivotY;
    float pivotZ;

    std::vector<Animation>* animations;

    Limb(std::vector<TexturedModel*>* models, float pivotX, float pivotY, float pivotZ, Body* body, Limb* limb);

    void step();

    void update(float time);

    std::vector<TexturedModel*>* getModels();
};
#endif
