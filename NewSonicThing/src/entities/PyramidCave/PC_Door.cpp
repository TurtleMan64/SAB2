#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "pcdoor.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <vector>
#include <iostream>

std::vector<TexturedModel*> PC_Door::models;
CollisionModel* PC_Door::cmOriginal = nullptr;

PC_Door::PC_Door()
{
    
}

PC_Door::PC_Door(float x, float y, float z, float yRot, int idx)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotY = yRot;
    id = idx;
    scale = 1.05f;

    updateTransformationMatrix();

    collideModelOriginal = PC_Door::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModelWithScale();
}

void PC_Door::step()
{
    extern float dt;

    if (moveTimer >= 0.0f  && moveTimer < 5.0f)
    {
        moveTimer += dt;

        position.y -= 20*dt;

        updateTransformationMatrix();
        updateCollisionModelWithScale();
    }
}

void PC_Door::activateFromSwitch(int switchId)
{
    if (switchId == id)
    {
        moveTimer = 0.0f;
    }
}

std::vector<TexturedModel*>* PC_Door::getModels()
{
    return &PC_Door::models;
}

void PC_Door::loadStaticModels()
{
    if (PC_Door::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading PC_Door static models...\n");
    #endif

    ModelLoader::loadModel(&PC_Door::models, "res/Models/Levels/PyramidCave/Door/", "Door");

    if (PC_Door::cmOriginal == nullptr)
    {
        PC_Door::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/PyramidCave/Door/", "Door");
    }
}

void PC_Door::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting PC_Door static models...\n");
    #endif

    Entity::deleteModels(&PC_Door::models);
    Entity::deleteCollisionModel(&PC_Door::cmOriginal);
}
