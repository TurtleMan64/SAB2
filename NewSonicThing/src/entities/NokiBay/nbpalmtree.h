#ifndef NBPALMTREE_H
#define NBPALMTREE_H

class TexturedModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class NB_Palmtree : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    NB_Palmtree();
    NB_Palmtree(float x, float y, float z, float rotY);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
