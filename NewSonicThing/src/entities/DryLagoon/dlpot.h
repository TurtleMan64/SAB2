#ifndef DL_POT_H
#define DL_POT_H

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class DL_Pot : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;

    static CollisionModel* cmOriginal;

public:
    DL_Pot(float x, float y, float z, float rot);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
