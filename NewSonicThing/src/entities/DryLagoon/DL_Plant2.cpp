#include "dlplant2.h"
#include "../entity.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../objLoader/objLoader.h"

std::list<TexturedModel*> DL_Plant2::models;

DL_Plant2::DL_Plant2()
{

}

DL_Plant2::DL_Plant2(
    float x, float y, float z,
    float rotX, float rotY, float rotZ,
    float scale)
{
    position.set(x, y, z);
    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;
    this->scale = scale;
    updateTransformationMatrixYXZ();
}

void DL_Plant2::step()
{
   
}

std::list<TexturedModel*>* DL_Plant2::getModels()
{
    return &DL_Plant2::models;
}

void DL_Plant2::loadStaticModels()
{
    if (DL_Plant2::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading DL_Plant2 static models...\n");
    #endif

    ObjLoader::loadModel(&DL_Plant2::models, "res/Models/Levels/DryLagoon/Objects/", "Flower");
}

void DL_Plant2::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting DL_Plant2 static models...\n");
    #endif

    Entity::deleteModels(&DL_Plant2::models);
}
