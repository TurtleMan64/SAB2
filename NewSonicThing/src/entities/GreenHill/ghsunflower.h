#ifndef GHSUNFLOWER_H
#define GHSUNFLOWER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"

class GH_Sunflower : public Entity
{
private:
    static std::list<TexturedModel*> modelsTop;
    static std::list<TexturedModel*> modelsTop2;
    static std::list<TexturedModel*> modelsBase;

    Dummy* petals = nullptr;

    float timer = 0.0f;

public:
    GH_Sunflower();
    GH_Sunflower(float x, float y, float z, float rotY, float scale, std::list<Entity*>* entityListToAdd);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
