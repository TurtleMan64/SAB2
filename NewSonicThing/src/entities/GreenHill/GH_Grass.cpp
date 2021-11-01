#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "ghgrass.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"

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
    std::fprintf(stdout, "Loading GH_Grass static models...\n");
    #endif

    ObjLoader::loadModel(&GH_Grass::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillGrass");
}

void GH_Grass::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting GH_Grass static models...\n");
    #endif

    Entity::deleteModels(&GH_Grass::models);
}
