#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "nbwaterplatform.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> NB_WaterPlatform::models;
CollisionModel* NB_WaterPlatform::cmOriginal = nullptr;

NB_WaterPlatform::NB_WaterPlatform()
{
    
}

NB_WaterPlatform::NB_WaterPlatform(float x, float y, float z, float rotY)
{
    this->position.x = x;
    this->position.y = y - 15;
    this->position.z = z;
    this->rotX = 0;
    this->rotY = rotY;
    this->rotZ = 0;
    this->scale = 1;
    this->visible = true;
    updateTransformationMatrix();

    collideModelOriginal = NB_WaterPlatform::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void NB_WaterPlatform::step()
{

}

std::list<TexturedModel*>* NB_WaterPlatform::getModels()
{
    return &NB_WaterPlatform::models;
}

void NB_WaterPlatform::loadStaticModels()
{
    if (NB_WaterPlatform::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading NB_WaterPlatform static models...\n");
    #endif

    ObjLoader::loadModel(&NB_WaterPlatform::models, "res/Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatform");

    if (NB_WaterPlatform::cmOriginal == nullptr)
    {
        NB_WaterPlatform::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatformCollision");
    }
}

void NB_WaterPlatform::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting NB_WaterPlatform static models...\n");
    #endif

    Entity::deleteModels(&NB_WaterPlatform::models);
    Entity::deleteCollisionModel(&NB_WaterPlatform::cmOriginal);
}
