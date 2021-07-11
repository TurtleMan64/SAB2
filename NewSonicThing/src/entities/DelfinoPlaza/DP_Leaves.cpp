#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "dpleaves.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> DP_Leaves::models;
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

std::list<TexturedModel*>* DP_Leaves::getModels()
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
    std::fprintf(stdout, "Loading DP_Leaves static models...\n");
    #endif

    ObjLoader::loadModel(&DP_Leaves::models, "res/Models/Levels/DelfinoPlaza/PalmLeaves/", "PalmLeaves");

    if (DP_Leaves::cmOriginal == nullptr)
    {
        DP_Leaves::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/DelfinoPlaza/PalmLeaves/", "PalmLeaves");
    }
}

void DP_Leaves::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting DP_Leaves static models...\n");
    #endif

    Entity::deleteModels(&DP_Leaves::models);
    Entity::deleteCollisionModel(&DP_Leaves::cmOriginal);
}
