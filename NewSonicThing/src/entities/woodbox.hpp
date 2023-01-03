#ifndef WOODBOX_H
#define WOODBOX_H

#include <list>
#include "entity.hpp"
#include "collideableobject.hpp"

class TexturedModel;

class WoodBox : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;

    static CollisionModel* cmOriginal;

public:
    WoodBox(float x, float y, float z, float rot);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
