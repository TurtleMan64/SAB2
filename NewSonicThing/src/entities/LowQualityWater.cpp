#include <glad/glad.h>
#include <string>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "lowqualitywater.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> LowQualityWater::models;

LowQualityWater::LowQualityWater()
{

}

LowQualityWater::LowQualityWater(
    float x, float y, float z, 
    float r, float g, float b, float a)
{
    position.x = x;
    position.y = y;
    position.z = z;
    baseColour.set(r, g, b);
    baseAlpha = a;
    updateTransformationMatrix();
}

void LowQualityWater::step()
{

}

std::list<TexturedModel*>* LowQualityWater::getModels()
{
    return &LowQualityWater::models;
}

void LowQualityWater::loadStaticModels()
{
    if (LowQualityWater::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading LowQualityWater static models...\n");
    #endif

    ModelLoader::loadModel(&LowQualityWater::models, "res/Models/Objects/Water/", "LowQualityWater");
}

void LowQualityWater::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting LowQualityWater models...\n");
    #endif

    Entity::deleteModels(&LowQualityWater::models);
}
