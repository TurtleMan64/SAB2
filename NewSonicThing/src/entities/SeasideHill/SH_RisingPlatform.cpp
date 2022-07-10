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

std::list<TexturedModel*> SH_RisingPlatform::modelsSmall;
std::list<TexturedModel*> SH_RisingPlatform::modelsMedium;
std::list<TexturedModel*> SH_RisingPlatform::modelsLarge;
CollisionModel* SH_RisingPlatform::cmSmallOriginal = nullptr;
CollisionModel* SH_RisingPlatform::cmMediumOriginal = nullptr;
CollisionModel* SH_RisingPlatform::cmLargeOriginal = nullptr;

SH_RisingPlatform::SH_RisingPlatform(float x, float y, float z, float rotY, int t, float distance, float tX, float tZ, float tR)
{
    position.set(x, y + distance, z);
    this->rotY = rotY;
    originalY = position.y;

    scale = 1;
    visible = true;

    type = Maths::clamp(0, t, 2);
    movingDistance = -distance;
    triggerX = tX;
    triggerZ = tZ;
    triggerR = tR;

    updateTransformationMatrix();

    switch (type)
    {
        case 0:  collideModelOriginal = SH_RisingPlatform::cmSmallOriginal;  break;
        case 1:  collideModelOriginal = SH_RisingPlatform::cmMediumOriginal; break;
        default: collideModelOriginal = SH_RisingPlatform::cmLargeOriginal;  break;
    }
    collideModelTransformed = collideModelOriginal->duplicateMe();
    CollisionChecker::addCollideModel(collideModelTransformed);

    // don't actually move the collision. just move the visible model.
    position.y = originalY + movingDistance;
    updateCollisionModel();

    //temp: disable the moving animation
    updateTransformationMatrix();
    triggerTimer = 1.0f;

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
    switch (type)
    {
        case 0:  return &SH_RisingPlatform::modelsSmall;
        case 1:  return &SH_RisingPlatform::modelsMedium;
        default: return &SH_RisingPlatform::modelsLarge;
    }
}

void SH_RisingPlatform::loadStaticModels()
{
    if (SH_RisingPlatform::modelsSmall.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SH_RisingPlatform static models...\n");
    #endif

    ModelLoader::loadModel(&SH_RisingPlatform::modelsSmall,  "res/Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatformSmall");
    ModelLoader::loadModel(&SH_RisingPlatform::modelsMedium, "res/Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatformMedium");
    ModelLoader::loadModel(&SH_RisingPlatform::modelsLarge,  "res/Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatformLarge");

    if (SH_RisingPlatform::cmSmallOriginal == nullptr)
    {
        SH_RisingPlatform::cmSmallOriginal = ModelLoader::loadCollisionModel("Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatformSmallCollision");
    }
    if (SH_RisingPlatform::cmMediumOriginal == nullptr)
    {
        SH_RisingPlatform::cmMediumOriginal = ModelLoader::loadCollisionModel("Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatformMediumCollision");
    }
    if (SH_RisingPlatform::cmLargeOriginal == nullptr)
    {
        SH_RisingPlatform::cmLargeOriginal = ModelLoader::loadCollisionModel("Models/Levels/SeasideHill/Objects/RisingPlatform/", "RisingPlatformLargeCollision");
    }
}

void SH_RisingPlatform::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SH_RisingPlatform static models...\n");
    #endif

    Entity::deleteModels(&SH_RisingPlatform::modelsSmall);
    Entity::deleteModels(&SH_RisingPlatform::modelsMedium);
    Entity::deleteModels(&SH_RisingPlatform::modelsLarge);
    Entity::deleteCollisionModel(&SH_RisingPlatform::cmSmallOriginal);
    Entity::deleteCollisionModel(&SH_RisingPlatform::cmMediumOriginal);
    Entity::deleteCollisionModel(&SH_RisingPlatform::cmLargeOriginal);
}
