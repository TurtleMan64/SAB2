#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "dppalmtree.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> DP_Palmtree::models;
CollisionModel* DP_Palmtree::cmOriginal = nullptr;

DP_Palmtree::DP_Palmtree()
{
    
}

DP_Palmtree::DP_Palmtree(float x, float y, float z, float rotY)
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

    collideModelOriginal = DP_Palmtree::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void DP_Palmtree::step()
{

}

std::list<TexturedModel*>* DP_Palmtree::getModels()
{
    return &DP_Palmtree::models;
}

void DP_Palmtree::loadStaticModels()
{
    if (DP_Palmtree::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading DP_Palmtree static models...\n");
    #endif

    ObjLoader::loadModel(&DP_Palmtree::models, "res/Models/Levels/DelfinoPlaza/Palm/", "Palm");

    if (DP_Palmtree::cmOriginal == nullptr)
    {
        DP_Palmtree::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/DelfinoPlaza/Palm/", "Collision");
    }
}

void DP_Palmtree::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting DP_Palmtree static models...\n");
    #endif

    Entity::deleteModels(&DP_Palmtree::models);
    Entity::deleteCollisionModel(&DP_Palmtree::cmOriginal);
}
