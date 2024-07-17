#ifndef BOSS_BOMB_H
#define BOSS_BOMB_H

#include <vector>
#include "../entity.hpp"
#include "../../toolbox/vector.hpp"

class TexturedModel;

class Boss_Bomb : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    static int bombsRemaining;

    Vector3f c1p1;
    Vector3f c1p2;
    Vector3f c2p1;
    Vector3f c2p2;
    Vector3f c3p1;
    Vector3f c3p2;

public:
    Boss_Bomb(float x, float y, float z, float rotY);
    ~Boss_Bomb();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
