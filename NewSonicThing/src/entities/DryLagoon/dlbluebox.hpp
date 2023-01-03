#ifndef DL_BLUEBOX_H
#define DL_BLUEBOX_H

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;

class DL_BlueBox : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;

    static CollisionModel* cmOriginal;

public:
    DL_BlueBox(float x, float y, float z, float rot);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
