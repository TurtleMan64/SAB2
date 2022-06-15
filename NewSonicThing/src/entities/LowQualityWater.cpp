#include <glad/glad.h>
#include <string>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "lowqualitywater.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> LowQualityWater::models;

LowQualityWater::LowQualityWater()
{

}

LowQualityWater::LowQualityWater(
    int x, int z, 
    float r, float g, float b, float a)
{
    this->x = x;
    this->z = z;
    //position.x = x;
    //position.y = y;
    //position.z = z;
    baseColor.set(r, g, b);
    baseAlpha = a;
    updateTransformationMatrix();
}

void LowQualityWater::step()
{
    if (Global::waterHeight != position.y)
    {
        position.y = Global::waterHeight;
        updateTransformationMatrix();
    }

    int centerX = (int)std::round(Global::gameMainPlayer->position.x / 8000.0f);
    int centerZ = (int)std::round(Global::gameMainPlayer->position.z / 8000.0f);

    int newX = centerX + x;
    int newZ = centerZ + z;
    int currentX = (int)std::round(position.x / 8000.0f);
    int currentZ = (int)std::round(position.z / 8000.0f);

    if (newX != currentX || newZ != currentZ)
    {
        position.x = newX * 8000.0f;
        position.z = newZ * 8000.0f;
        updateTransformationMatrix();
    }
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
