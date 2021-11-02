#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghfloatingplatform.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> GH_FloatingPlatform::models;
CollisionModel* GH_FloatingPlatform::cmOriginal = nullptr;

GH_FloatingPlatform::GH_FloatingPlatform()
{
    
}

GH_FloatingPlatform::GH_FloatingPlatform(float x, float y, float z, float rotY, float scale)
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

    collideModelOriginal = GH_FloatingPlatform::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void GH_FloatingPlatform::step()
{

}

std::list<TexturedModel*>* GH_FloatingPlatform::getModels()
{
    return &GH_FloatingPlatform::models;
}

void GH_FloatingPlatform::loadStaticModels()
{
    if (GH_FloatingPlatform::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading GH_FloatingPlatform static models...\n");
    #endif

    ModelLoader::loadModel(&GH_FloatingPlatform::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillFallingPlatform");

    if (GH_FloatingPlatform::cmOriginal == nullptr)
    {
        GH_FloatingPlatform::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/GreenHillZone/Objects/", "GreenHillFallingPlatform");
    }
}

void GH_FloatingPlatform::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting GH_FloatingPlatform static models...\n");
    #endif

    Entity::deleteModels(&GH_FloatingPlatform::models);
    Entity::deleteCollisionModel(&GH_FloatingPlatform::cmOriginal);
}
