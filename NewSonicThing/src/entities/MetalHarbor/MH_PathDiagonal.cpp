#include "mhpathdiagonal.h"
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

std::list<TexturedModel*> MH_PathDiagonal::models;
CollisionModel* MH_PathDiagonal::cmOriginal;

MH_PathDiagonal::MH_PathDiagonal()
{

}

MH_PathDiagonal::MH_PathDiagonal(float x, float y, float z, float rotY)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    this->rotY = rotY;
    rotZ = 0;

    scale = 1;
    visible = true;
    
    updateTransformationMatrix();

    collideModelOriginal = MH_PathDiagonal::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void MH_PathDiagonal::step()
{
    
}

std::list<TexturedModel*>* MH_PathDiagonal::getModels()
{
    return &MH_PathDiagonal::models;
}

void MH_PathDiagonal::loadStaticModels()
{
    if (MH_PathDiagonal::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading MH_PathDiagonal static models...\n");
    #endif

    loadModel(&MH_PathDiagonal::models, "res/Models/Levels/MetalHarbor/", "PathDiagonal");

    if (MH_PathDiagonal::cmOriginal == nullptr)
    {
        MH_PathDiagonal::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "PathDiagonalCollision");
    }
}

void MH_PathDiagonal::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting MH_PathDiagonal static models...\n");
    #endif

    Entity::deleteModels(&MH_PathDiagonal::models);
    Entity::deleteCollisionModel(&MH_PathDiagonal::cmOriginal);
}
