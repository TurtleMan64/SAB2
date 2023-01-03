#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "rhramp.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../entities/playersonic.hpp"
#include "../../toolbox/maths.hpp"
#include "../camera.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>
#include <algorithm>

std::vector<TexturedModel*> RH_Ramp::models;
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

std::vector<TexturedModel*>* RH_Ramp::getModels()
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
    printf("Loading RH_Ramp static models...\n");
    #endif

    ModelLoader::loadModel(&RH_Ramp::models, "res/Models/Levels/RadicalHighway/Objects/", "Ramp");

    if (RH_Ramp::cmOriginal == nullptr)
    {
        RH_Ramp::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/RadicalHighway/Objects/", "Ramp");
    }
}

void RH_Ramp::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting RH_Ramp static models...\n");
    #endif

    Entity::deleteModels(&RH_Ramp::models);
    Entity::deleteCollisionModel(&RH_Ramp::cmOriginal);
}
