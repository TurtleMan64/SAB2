#ifndef DPPALMTREE_H
#define DPPALMTREE_H

class TexturedModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class DP_Palmtree : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    DP_Palmtree();
    DP_Palmtree(float x, float y, float z, float rotY);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
