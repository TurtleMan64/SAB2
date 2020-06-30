#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "rhramp.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../entities/playersonic.h"
#include "../../toolbox/maths.h"
#include "../camera.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <list>
#include <iostream>
#include <algorithm>

std::list<TexturedModel*> RH_Ramp::models;
CollisionModel* RH_Ramp::cmOriginal = nullptr;

RH_Ramp::RH_Ramp()
{
    
}

RH_Ramp::RH_Ramp(float x, float y, float z, float rotYNew, float rotZNew)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = rotYNew;
    rotZ = rotZNew;
    updateTransformationMatrix();

    collideModelOriginal = RH_Ramp::cmOriginal;
    collideModelTransformed = RH_Ramp::cmOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModelWithZ();

}

void RH_Ramp::step()
{

}

std::list<TexturedModel*>* RH_Ramp::getModels()
{
    return &RH_Ramp::models;
}

void RH_Ramp::loadStaticModels()
{
    if (RH_Ramp::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading RH_Ramp static models...\n");
    #endif

    ObjLoader::loadModel(&RH_Ramp::models, "res/Models/Levels/RadicalHighway/Objects/", "Ramp");

    if (RH_Ramp::cmOriginal == nullptr)
    {
        RH_Ramp::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/RadicalHighway/Objects/", "Ramp");
    }
}

void RH_Ramp::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting RH_Ramp static models...\n");
    #endif

    Entity::deleteModels(&RH_Ramp::models);
    Entity::deleteCollisionModel(&RH_Ramp::cmOriginal);
}
