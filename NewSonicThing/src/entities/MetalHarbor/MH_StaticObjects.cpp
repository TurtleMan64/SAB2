#include "mhstaticobjects.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../dummy.hpp"

std::list<TexturedModel*> MH_StaticObjects::models;
std::list<TexturedModel*> MH_StaticObjects::modelsVertexColors;
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
    
    setupModelVertexColors();
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

    ModelLoader::loadModel(&MH_StaticObjects::models, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjects");
    ModelLoader::loadModel(&MH_StaticObjects::modelsVertexColors, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsVertexColors");
    ModelLoader::loadModel(&MH_StaticObjects::modelsTransparent, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsTransparent");

    setModelsRenderOrder(&MH_StaticObjects::modelsTransparent, 1);

    if (MH_StaticObjects::cmOriginal == nullptr)
    {
        MH_StaticObjects::cmOriginal = ModelLoader::loadCollisionModel("Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsCollision");
    }
}

void MH_StaticObjects::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting MH_StaticObjects static models...\n");
    #endif

    Entity::deleteModels(&MH_StaticObjects::models);
    Entity::deleteModels(&MH_StaticObjects::modelsVertexColors);
    Entity::deleteModels(&MH_StaticObjects::modelsTransparent);
    Entity::deleteCollisionModel(&MH_StaticObjects::cmOriginal);
}

void MH_StaticObjects::setupModelVertexColors()
{
    modelVertexColors = new Dummy(&MH_StaticObjects::modelsVertexColors);
    modelVertexColors->setVisible(true);
    INCR_NEW("Entity");
    Global::addEntity(modelVertexColors);
}

void MH_StaticObjects::setupModelTransparent()
{
    modelTransparent = new Dummy(&MH_StaticObjects::modelsTransparent);
    modelTransparent->setVisible(true);
    INCR_NEW("Entity");
    Global::addEntity(modelTransparent);
}
