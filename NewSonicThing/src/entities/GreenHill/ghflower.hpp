#ifndef GHFLOWER_H
#define GHFLOWER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class GH_Flower : public Entity
{
private:
    static std::vector<TexturedModel*> modelsTop;
    static std::vector<TexturedModel*> modelsBase;

    Dummy* petals = nullptr;

    float timer = 0.0f;

public:
    GH_Flower();
    GH_Flower(float x, float y, float z, float rotY, float scale, std::list<Entity*>* entityListToAdd);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
