#include "shrisingplatform.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../controllableplayer.hpp"
#include "../../audio/audioplayer.hpp"

std::list<TexturedModel*> SH_RisingPlatform::models;
CollisionModel* SH_RisingPlatform::cmOriginal = nullptr;

SH_RisingPlatform::SH_RisingPlatform(float x, float y, float z, int t, float distance, float tX, float tZ, float tR)
{
    position.set(x, y, z);
    originalY = y;

    scale = 1;
    visible = true;

    type = t;
    movingDistance = -distance;
    triggerX = tX;
    triggerZ = tZ;
    triggerR = tR;

    updateTransformationMatrix();

    collideModelOriginal = SH_RisingPlatform::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    // don't actually move the collision. just move the visible model.
    position.y = originalY + movingDistance;
    updateCollisionModel();
    position.y = originalY;
}

void SH_RisingPlatform::step()
{
    if (triggerTimer <= 0.0f)
    {
        Vector3f diff = position - Global::gameMainPlayer->position;
        diff.y = 0.0f;
        if (diff.lengthSquared() < 300*300)
        {
            triggerTimer = 0.00001f;
            //todo play sound
        }
    }
    else if (triggerTimer < 1.0f)
    {
        extern float dt;
        triggerTimer += dt;

        if (triggerTimer >= 1.0f)
        {
            position.y = originalY + movingDistance;
        }
        else
        {
            position.y = originalY + movingDistance*(triggerTimer);
        }

        updateTransformationMatrix();
    }
}

std::list<TexturedModel*>* SH_RisingPlatform::getModels()
{
    return &SH_RisingPlatform::models;
}

void SH_RisingPlatform::loadStaticModels()
{
    if (SH_RisingPlatform::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SH_RisingPlatform static models...\n");
    #endif

    ModelLoader::loadModel(&SH_RisingPlatform::models, "res/Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatform");

    if (SH_RisingPlatform::cmOriginal == nullptr)
    {
        SH_RisingPlatform::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatform");
    }
}

void SH_RisingPlatform::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SH_RisingPlatform static models...\n");
    #endif

    Entity::deleteModels(&SH_RisingPlatform::models);
    Entity::deleteCollisionModel(&SH_RisingPlatform::cmOriginal);
}
