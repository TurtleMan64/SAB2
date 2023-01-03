#include "dlplant2.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"

std::vector<TexturedModel*> DL_Plant2::models;

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

std::vector<TexturedModel*>* DL_Plant2::getModels()
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
    printf("Loading DL_Plant2 static models...\n");
    #endif

    ModelLoader::loadModel(&DL_Plant2::models, "res/Models/Levels/DryLagoon/Objects/", "Flower");
}

void DL_Plant2::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DL_Plant2 static models...\n");
    #endif

    Entity::deleteModels(&DL_Plant2::models);
}
