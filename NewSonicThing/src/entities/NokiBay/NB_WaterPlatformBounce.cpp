#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "nbwaterplatformbounce.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../../audio/audioplayer.hpp"
#include <list>
#include <iostream>

std::vector<TexturedModel*> NB_WaterPlatformBounce::models;
CollisionModel* NB_WaterPlatformBounce::cmOriginal = nullptr;

NB_WaterPlatformBounce::NB_WaterPlatformBounce()
{
    
}

NB_WaterPlatformBounce::NB_WaterPlatformBounce(float x, float y, float z, float rotY)
{
    this->position.x = x;
    this->position.y = y - 16.453f;
    this->position.z = z;
    this->rotX = 0;
    this->rotY = rotY;
    this->rotZ = 0;
    this->scale = 1;
    this->visible = true;
    updateTransformationMatrix();

    collideModelOriginal = NB_WaterPlatformBounce::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void NB_WaterPlatformBounce::step()
{

}

std::vector<TexturedModel*>* NB_WaterPlatformBounce::getModels()
{
    return &NB_WaterPlatformBounce::models;
}

void NB_WaterPlatformBounce::loadStaticModels()
{
    if (NB_WaterPlatformBounce::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading NB_WaterPlatformBounce static models...\n");
    #endif

    ModelLoader::loadModel(&NB_WaterPlatformBounce::models, "res/Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatformBounce");

    if (NB_WaterPlatformBounce::cmOriginal == nullptr)
    {
        NB_WaterPlatformBounce::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatformBounceCollision");
    }
}

void NB_WaterPlatformBounce::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting NB_WaterPlatformBounce static models...\n");
    #endif

    Entity::deleteModels(&NB_WaterPlatformBounce::models);
    Entity::deleteCollisionModel(&NB_WaterPlatformBounce::cmOriginal);
}
