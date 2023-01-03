#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "dpleaves.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> DP_Leaves::models;
CollisionModel* DP_Leaves::cmOriginal = nullptr;

DP_Leaves::DP_Leaves()
{
    
}

DP_Leaves::DP_Leaves(float x, float y, float z, float rotY)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    this->rotX = 0;
    this->rotY = rotY;
    this->rotZ = 0;
    this->scale = 1;
    this->visible = true;
    updateTransformationMatrix();

    collideModelOriginal = DP_Leaves::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void DP_Leaves::step()
{
    
}

std::vector<TexturedModel*>* DP_Leaves::getModels()
{
    return &DP_Leaves::models;
}

void DP_Leaves::loadStaticModels()
{
    if (DP_Leaves::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DP_Leaves static models...\n");
    #endif

    ModelLoader::loadModel(&DP_Leaves::models, "res/Models/Levels/DelfinoPlaza/PalmLeaves/", "PalmLeaves");

    if (DP_Leaves::cmOriginal == nullptr)
    {
        DP_Leaves::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/DelfinoPlaza/PalmLeaves/", "PalmLeaves");
    }
}

void DP_Leaves::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DP_Leaves static models...\n");
    #endif

    Entity::deleteModels(&DP_Leaves::models);
    Entity::deleteCollisionModel(&DP_Leaves::cmOriginal);
}
