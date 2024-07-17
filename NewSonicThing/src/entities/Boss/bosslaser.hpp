#ifndef BOSS_LASER_H
#define BOSS_LASER_H

#include <vector>
#include "../entity.hpp"
#include "../../toolbox/vector.hpp"

class TexturedModel;

class Boss_Laser : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    Vector3f dir;
    Vector3f normal;

public:
    Boss_Laser(float x, float y, float z);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
