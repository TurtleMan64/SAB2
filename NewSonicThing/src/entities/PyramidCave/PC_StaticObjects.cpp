#include "pcstaticobjects.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../../animation/body.hpp"

std::list<TexturedModel*> PC_StaticObjects::models;
std::list<TexturedModel*> PC_StaticObjects::modelsVertexColors;
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
    
    setupModelVertexColors();
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

    ModelLoader::loadModel(&PC_StaticObjects::models, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjects");
    ModelLoader::loadModel(&PC_StaticObjects::modelsVertexColors, "res/Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsVertexColors");
    ModelLoader::loadModel(&PC_StaticObjects::modelsTransparent, "res/Models/Objects/LevelSpecific/PyramidCave/StaticObjects/", "HallwayLight");

    setModelsRenderOrder(&PC_StaticObjects::modelsTransparent, 1);

    if (PC_StaticObjects::cmOriginal == nullptr)
    {
        PC_StaticObjects::cmOriginal = ModelLoader::loadCollisionModel("Models/Objects/MetalHarbor/StaticObjects/", "StaticObjectsCollision");
    }
}

void PC_StaticObjects::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting PC_StaticObjects static models...\n");
    #endif

    Entity::deleteModels(&PC_StaticObjects::models);
    Entity::deleteModels(&PC_StaticObjects::modelsVertexColors);
    Entity::deleteModels(&PC_StaticObjects::modelsTransparent);
    Entity::deleteCollisionModel(&PC_StaticObjects::cmOriginal);
}

void PC_StaticObjects::setupModelVertexColors()
{
    modelVertexColors = new Body(&PC_StaticObjects::modelsVertexColors);
    modelVertexColors->setVisible(true);
    INCR_NEW("Entity");
    Global::addEntity(modelVertexColors);
}

void PC_StaticObjects::setupModelTransparent()
{
    modelTransparent = new Body(&PC_StaticObjects::modelsTransparent);
    modelTransparent->setVisible(true);
    INCR_NEW("Entity");
    Global::addEntity(modelTransparent);
}
