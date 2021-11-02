#ifndef BOMB_H
#define BOMB_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class Bomb : public Entity
{
private:
    static std::list<TexturedModel*> models;

    Vector3f vel;
    bool moves = true;
    float timer = 0.0f;

public:
    Bomb();
    Bomb(float x, float y, float z, float xVel, float yVel, float zVel);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
