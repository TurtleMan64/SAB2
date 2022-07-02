#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghtotem.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> GH_Totem::models;
CollisionModel* GH_Totem::cmOriginal = nullptr;

GH_Totem::GH_Totem()
{
    
}

GH_Totem::GH_Totem(float x, float y, float z, float rotY, float scale)
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

    collideModelOriginal = GH_Totem::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void GH_Totem::step()
{

}

std::list<TexturedModel*>* GH_Totem::getModels()
{
    return &GH_Totem::models;
}

void GH_Totem::loadStaticModels()
{
    if (GH_Totem::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GH_Totem static models...\n");
    #endif

    ModelLoader::loadModel(&GH_Totem::models, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillTotem");

    if (GH_Totem::cmOriginal == nullptr)
    {
        GH_Totem::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/GreenHillZone/Objects/", "GreenHillTotemCollision");
    }
}

void GH_Totem::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_Totem static models...\n");
    #endif

    Entity::deleteModels(&GH_Totem::models);
    Entity::deleteCollisionModel(&GH_Totem::cmOriginal);
}
