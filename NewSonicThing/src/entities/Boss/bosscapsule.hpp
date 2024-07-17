#ifndef BOSS_CAPSULE_H
#define BOSS_CAPSULE_H

#include <vector>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;

class Boss_Capsule : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;

    static CollisionModel* cmOriginal;

    float timer = 0.0f;

public:
    Boss_Capsule(float x, float y, float z);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
