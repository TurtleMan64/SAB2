#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghrock.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> GH_Rock::models;
CollisionModel* GH_Rock::cmOriginal = nullptr;

GH_Rock::GH_Rock()
{
    
}

GH_Rock::GH_Rock(float x, float y, float z, float rotY, float scale)
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

    collideModelOriginal = GH_Rock::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void GH_Rock::step()
{

}

std::list<TexturedModel*>* GH_Rock::getModels()
{
    return &GH_Rock::models;
}

void GH_Rock::loadStaticModels()
{
    if (GH_Rock::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GH_Rock static models...\n");
    #endif

    ModelLoader::loadModel(&GH_Rock::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillRock");

    if (GH_Rock::cmOriginal == nullptr)
    {
        GH_Rock::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/GreenHillZone/Objects/", "GreenHillRockCollision");
    }
}

void GH_Rock::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_Rock static models...\n");
    #endif

    Entity::deleteModels(&GH_Rock::models);
    Entity::deleteCollisionModel(&GH_Rock::cmOriginal);
}
