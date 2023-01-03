#ifndef LOWQUALITYWATER_H
#define LOWQUALITYWATER_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include <string>

class LowQualityWater : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    int x = 0;
    int z = 0;

public:
    LowQualityWater();

    LowQualityWater(
        int x, int z, 
        float r, float g, float b, float a);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
