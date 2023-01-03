#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "nbwaterplatform.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> NB_WaterPlatform::models;
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

std::vector<TexturedModel*>* NB_WaterPlatform::getModels()
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
    printf("Loading NB_WaterPlatform static models...\n");
    #endif

    ModelLoader::loadModel(&NB_WaterPlatform::models, "res/Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatform");

    if (NB_WaterPlatform::cmOriginal == nullptr)
    {
        NB_WaterPlatform::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatformCollision");
    }
}

void NB_WaterPlatform::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting NB_WaterPlatform static models...\n");
    #endif

    Entity::deleteModels(&NB_WaterPlatform::models);
    Entity::deleteCollisionModel(&NB_WaterPlatform::cmOriginal);
}
