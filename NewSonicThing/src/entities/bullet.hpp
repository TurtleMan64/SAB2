#ifndef BULLET_H
#define BULLET_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class Bullet : public Entity
{
private:
    static std::list<TexturedModel*> models;

    float timeUntilDeath;
    Vector3f vel;

public:
    Bullet();
    Bullet(Vector3f* pos, Vector3f* vel, float timeToLive);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
