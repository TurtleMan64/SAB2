#ifndef NBPALMTREE_H
#define NBPALMTREE_H

class TexturedModel;

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class NB_Palmtree : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    NB_Palmtree();
    NB_Palmtree(float x, float y, float z, float rotY);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
