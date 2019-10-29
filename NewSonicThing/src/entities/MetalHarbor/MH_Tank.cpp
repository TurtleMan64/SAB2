#include "mhtank.h"
#include "../entity.h"
#include "../camera.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <algorithm>

std::list<TexturedModel*> MH_Tank::models;
CollisionModel* MH_Tank::cmOriginal;

MH_Tank::MH_Tank()
{

}

MH_Tank::MH_Tank(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    scale = 1;
    visible = true;
    
    updateTransformationMatrix();

    collideModelOriginal = MH_Tank::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();
    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void MH_Tank::step()
{
    
}

std::list<TexturedModel*>* MH_Tank::getModels()
{
    return &MH_Tank::models;
}

void MH_Tank::loadStaticModels()
{
    if (MH_Tank::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading MH_Tank static models...\n");
    #endif

    loadModel(&MH_Tank::models, "res/Models/Levels/MetalHarbor/", "Tank");

    if (MH_Tank::cmOriginal == nullptr)
    {
        MH_Tank::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "TankCollision");
    }
}

void MH_Tank::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting MH_Tank static models...\n");
    #endif

    Entity::deleteModels(&MH_Tank::models);
    Entity::deleteCollisionModel(&MH_Tank::cmOriginal);
}
