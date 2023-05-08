#include "pcship.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"

#include <vector>

std::vector<TexturedModel*> PC_Ship::models;

PC_Ship::PC_Ship(float x, float y, float z, float rot)
{
    position.x = x;
    position.y = y;
    position.z = z;

    rotY = rot;

    updateTransformationMatrix();
}

void PC_Ship::step()
{
    extern float dt;

    timer += dt;

    rotX = sinf(timer)/2;

    transformationMatrix.setIdentity();
    transformationMatrix.translate(&position);
    transformationMatrix.rotate(rotY, &Y_AXIS);
    transformationMatrix.rotate(rotX, &X_AXIS);
}

std::vector<TexturedModel*>* PC_Ship::getModels()
{
    return &PC_Ship::models;
}

void PC_Ship::loadStaticModels()
{
    if (PC_Ship::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading PC_Ship static models...\n");
    #endif

    ModelLoader::loadModel(&PC_Ship::models, "res/Models/Levels/PyramidCave/Ship/", "Ship");
}

void PC_Ship::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting PC_Ship static models...\n");
    #endif

    Entity::deleteModels(&PC_Ship::models);
}
