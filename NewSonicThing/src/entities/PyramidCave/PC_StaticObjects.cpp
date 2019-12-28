#include "pcstaticobjects.h"
#include "../entity.h"
#include "../camera.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"
#include "../../animation/body.h"

#include <algorithm>

std::list<TexturedModel*> PC_StaticObjects::models;
std::list<TexturedModel*> PC_StaticObjects::modelsVertexColours;
std::list<TexturedModel*> PC_StaticObjects::modelsTransparent;
CollisionModel* PC_StaticObjects::cmOriginal;

PC_StaticObjects::PC_StaticObjects()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    scale = 1;
    visible = true;
    
    setupModelVertexColours();
    setupModelTransparent();

    updateTransformationMatrix();

    collideModelOriginal = PC_StaticObjects::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void PC_StaticObjects::step() {}

std::list<TexturedModel*>* PC_StaticObjects::getModels()
{
    return &PC_StaticObjects::models;
}

void PC_StaticObjects::loadStaticModels()
{
    if (PC_StaticObjects::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading PC_StaticObjects static models...\n");
    #endif

    loadModel(&PC_StaticObjects::models, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjects");
    loadModel(&PC_StaticObjects::modelsVertexColours, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsVertexColours");
    loadModel(&PC_StaticObjects::modelsTransparent, "res/Models/Objects/LevelSpecific/PyramidCave/StaticObjects/", "HallwayLight");

    if (PC_StaticObjects::cmOriginal == nullptr)
    {
        PC_StaticObjects::cmOriginal = loadCollisionModel("Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsCollision");
    }
}

void PC_StaticObjects::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting PC_StaticObjects static models...\n");
    #endif

    Entity::deleteModels(&PC_StaticObjects::models);
    Entity::deleteModels(&PC_StaticObjects::modelsVertexColours);
    Entity::deleteModels(&PC_StaticObjects::modelsTransparent);
    Entity::deleteCollisionModel(&PC_StaticObjects::cmOriginal);
}

void PC_StaticObjects::setupModelVertexColours()
{
    modelVertexColours = new Body(&PC_StaticObjects::modelsVertexColours);
    modelVertexColours->setVisible(true);
    INCR_NEW("Entity");
    Main_addEntity(modelVertexColours);
}

void PC_StaticObjects::setupModelTransparent()
{
    modelTransparent = new Body(&PC_StaticObjects::modelsTransparent);
    modelTransparent->setVisible(true);
    INCR_NEW("Entity");
    Main_addEntity(modelTransparent);
    modelTransparent->renderOrder = 1;
}
