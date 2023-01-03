#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghfallingplatform.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../../audio/audioplayer.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> GH_FallingPlatform::models;
CollisionModel* GH_FallingPlatform::cmOriginal = nullptr;

GH_FallingPlatform::GH_FallingPlatform()
{
    
}

GH_FallingPlatform::GH_FallingPlatform(float x, float y, float z, float rotY, float scale)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    this->rotY = rotY;
    rotZ = 0;
    this->scale = scale;
    visible = true;
    timer = 0.4f;
    updateTransformationMatrix();

    collideModelOriginal = GH_FallingPlatform::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void GH_FallingPlatform::step()
{
    extern float dt;

    float timerPrev = timer;

    if (collideModelTransformed->playerIsOn && timer == 0.4f)
    {
        timer = 0.39999f;
    }
                
    if (timer < 0.4f)
    {
        timer -= dt;
    }
                
    if (timer <= 0.0f && timerPrev > 0.0f)
    {
        AudioPlayer::play(0, &position);
    }
                
    if (timer <= 0.0f)
    {
        increasePosition(0, timer*4.6153846f*(dt*60), 0);
        updateCollisionModel();
        updateTransformationMatrix();
    }
                
    if (timer <= -10.0f)
    {
        CollisionChecker::deleteCollideModel(collideModelTransformed);
        collideModelTransformed = nullptr;
        Global::deleteChunkedEntity(this);
    }
}

std::vector<TexturedModel*>* GH_FallingPlatform::getModels()
{
    return &GH_FallingPlatform::models;
}

void GH_FallingPlatform::loadStaticModels()
{
    if (GH_FallingPlatform::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GH_FallingPlatform static models...\n");
    #endif

    ModelLoader::loadModel(&GH_FallingPlatform::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillFallingPlatform");

    if (GH_FallingPlatform::cmOriginal == nullptr)
    {
        GH_FallingPlatform::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/GreenHillZone/Objects/", "GreenHillFallingPlatform");
    }
}

void GH_FallingPlatform::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_FallingPlatform static models...\n");
    #endif

    Entity::deleteModels(&GH_FallingPlatform::models);
    Entity::deleteCollisionModel(&GH_FallingPlatform::cmOriginal);
}
