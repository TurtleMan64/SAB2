#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "nbwaterplatformbounce.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"
#include "../../audio/audioplayer.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> NB_WaterPlatformBounce::models;
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

std::list<TexturedModel*>* NB_WaterPlatformBounce::getModels()
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
    std::fprintf(stdout, "Loading NB_WaterPlatformBounce static models...\n");
    #endif

    ObjLoader::loadModel(&NB_WaterPlatformBounce::models, "res/Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatformBounce");

    if (NB_WaterPlatformBounce::cmOriginal == nullptr)
    {
        NB_WaterPlatformBounce::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/NokiBay/WaterPlatforms/", "WaterPlatformBounceCollision");
    }
}

void NB_WaterPlatformBounce::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting NB_WaterPlatformBounce static models...\n");
    #endif

    Entity::deleteModels(&NB_WaterPlatformBounce::models);
    Entity::deleteCollisionModel(&NB_WaterPlatformBounce::cmOriginal);
}
