#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "nbpalmtree.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> NB_Palmtree::models;
CollisionModel* NB_Palmtree::cmOriginal = nullptr;

NB_Palmtree::NB_Palmtree()
{
    
}

NB_Palmtree::NB_Palmtree(float x, float y, float z, float rotY)
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

    collideModelOriginal = NB_Palmtree::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void NB_Palmtree::step()
{

}

std::list<TexturedModel*>* NB_Palmtree::getModels()
{
    return &NB_Palmtree::models;
}

void NB_Palmtree::loadStaticModels()
{
    if (NB_Palmtree::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading NB_Palmtree static models...\n");
    #endif

    ModelLoader::loadModel(&NB_Palmtree::models, "res/Models/Levels/NokiBay/Palmtree/", "Palm");

    if (NB_Palmtree::cmOriginal == nullptr)
    {
        NB_Palmtree::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/NokiBay/Palmtree/", "Collision");
    }
}

void NB_Palmtree::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting NB_Palmtree static models...\n");
    #endif

    Entity::deleteModels(&NB_Palmtree::models);
    Entity::deleteCollisionModel(&NB_Palmtree::cmOriginal);
}
