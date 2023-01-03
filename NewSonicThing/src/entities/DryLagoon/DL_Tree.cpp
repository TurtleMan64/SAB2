#include "dltree.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"

std::vector<TexturedModel*> DL_Tree::models;

DL_Tree::DL_Tree()
{

}

DL_Tree::DL_Tree(
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

void DL_Tree::step()
{
   
}

std::vector<TexturedModel*>* DL_Tree::getModels()
{
    return &DL_Tree::models;
}

void DL_Tree::loadStaticModels()
{
    if (DL_Tree::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DL_Tree static models...\n");
    #endif

    ModelLoader::loadModel(&DL_Tree::models, "res/Models/Levels/DryLagoon/Objects/", "Tree");
}

void DL_Tree::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DL_Tree static models...\n");
    #endif

    Entity::deleteModels(&DL_Tree::models);
}
