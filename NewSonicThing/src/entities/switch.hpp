#ifndef SWITCH_H
#define SWITCH_H

class TexturedModel;

#include <vector>
#include "entity.hpp"

class Switch : public Entity
{
public:
    static std::vector<TexturedModel*> models;

private:
    bool activated = false;
    int id = -1;

public:
    Switch();
    Switch(float x, float y, float z, int id);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
