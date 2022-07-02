#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghgrass.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> GH_Grass::models;

GH_Grass::GH_Grass()
{
    
}

GH_Grass::GH_Grass(float x, float y, float z, float rotY, float scale)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    this->rotY = rotY;
    rotZ = 0;
    this->scale = scale;
    visible = true;
    updateTransformationMatrix();
}

void GH_Grass::step()
{

}

std::list<TexturedModel*>* GH_Grass::getModels()
{
    return &GH_Grass::models;
}

void GH_Grass::loadStaticModels()
{
    if (GH_Grass::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GH_Grass static models...\n");
    #endif

    ModelLoader::loadModel(&GH_Grass::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillGrass");
}

void GH_Grass::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_Grass static models...\n");
    #endif

    Entity::deleteModels(&GH_Grass::models);
}
