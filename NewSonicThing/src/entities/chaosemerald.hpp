#ifndef CHAOSEMERALD_H
#define CHAOSEMERALD_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class ChaosEmerald : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    bool found = false;
    int id = 0;
    float bobTimer = 0.0f;

public:
    ChaosEmerald();
    ChaosEmerald(float x, float y, float z, int id);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
