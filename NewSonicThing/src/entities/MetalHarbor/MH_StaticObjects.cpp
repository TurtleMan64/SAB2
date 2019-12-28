#include "mhstaticobjects.h"
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

std::list<TexturedModel*> MH_StaticObjects::models;
std::list<TexturedModel*> MH_StaticObjects::modelsVertexColours;
std::list<TexturedModel*> MH_StaticObjects::modelsTransparent;
CollisionModel* MH_StaticObjects::cmOriginal;

MH_StaticObjects::MH_StaticObjects()
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

    collideModelOriginal = MH_StaticObjects::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void MH_StaticObjects::step() {}

std::list<TexturedModel*>* MH_StaticObjects::getModels()
{
    return &MH_StaticObjects::models;
}

void MH_StaticObjects::loadStaticModels()
{
    if (MH_StaticObjects::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading MH_StaticObjects static models...\n");
    #endif

    loadModel(&MH_StaticObjects::models, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjects");
    loadModel(&MH_StaticObjects::modelsVertexColours, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsVertexColours");
    loadModel(&MH_StaticObjects::modelsTransparent, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsTransparent");


    if (MH_StaticObjects::cmOriginal == nullptr)
    {
        MH_StaticObjects::cmOriginal = loadCollisionModel("Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsCollision");
    }
}

void MH_StaticObjects::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting MH_StaticObjects static models...\n");
    #endif

    Entity::deleteModels(&MH_StaticObjects::models);
    Entity::deleteModels(&MH_StaticObjects::modelsVertexColours);
    Entity::deleteModels(&MH_StaticObjects::modelsTransparent);
    Entity::deleteCollisionModel(&MH_StaticObjects::cmOriginal);
}

void MH_StaticObjects::setupModelVertexColours()
{
    modelVertexColours = new Body(&MH_StaticObjects::modelsVertexColours);
    modelVertexColours->setVisible(true);
    INCR_NEW("Entity");
    Main_addEntity(modelVertexColours);
}

void MH_StaticObjects::setupModelTransparent()
{
    modelTransparent = new Body(&MH_StaticObjects::modelsTransparent);
    modelTransparent->setVisible(true);
    INCR_NEW("Entity");
    Main_addEntity(modelTransparent);
    modelTransparent->renderOrder = 1;
}
