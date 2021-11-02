#ifndef LOWQUALITYWATER_H
#define LOWQUALITYWATER_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include <string>

class LowQualityWater : public Entity
{
private:
    static std::list<TexturedModel*> models;

public:
    LowQualityWater();

    LowQualityWater(
        float x, float y, float z, 
        float r, float g, float b, float a);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
