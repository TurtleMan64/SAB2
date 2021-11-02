#ifndef DPLEAVES_H
#define DPLEAVES_H

class TexturedModel;

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class DP_Leaves : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    DP_Leaves();
    DP_Leaves(float x, float y, float z, float rotY);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
