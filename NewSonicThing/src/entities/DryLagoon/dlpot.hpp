#ifndef DL_POT_H
#define DL_POT_H

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;

class DL_Pot : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;

    static CollisionModel* cmOriginal;

public:
    DL_Pot(float x, float y, float z, float rot);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
