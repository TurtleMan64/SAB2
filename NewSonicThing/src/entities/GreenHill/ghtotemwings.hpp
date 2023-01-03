#ifndef GHTOTEMWINGS_H
#define GHTOTEMWINGS_H

class TexturedModel;
class CollisionModel;

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class GH_TotemWings : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    GH_TotemWings();
    GH_TotemWings(float x, float y, float z, float rotY, float scale);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
