#include "pcneonsign.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"

#include <vector>

std::vector<TexturedModel*> PC_NeonSign::models;

PC_NeonSign::PC_NeonSign(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;

    spinSpeed = 16*(Maths::random() + 0.1f);

    updateTransformationMatrix();
}

void PC_NeonSign::step()
{
    extern float dt;

    rotY += spinSpeed*dt;

    transformationMatrix.setIdentity();
    transformationMatrix.translate(&position);
    transformationMatrix.rotate(rotY, &Y_AXIS);
}

std::vector<TexturedModel*>* PC_NeonSign::getModels()
{
    return &PC_NeonSign::models;
}

void PC_NeonSign::loadStaticModels()
{
    if (PC_NeonSign::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading PC_NeonSign static models...\n");
    #endif

    ModelLoader::loadModel(&PC_NeonSign::models, "res/Models/Levels/PyramidCave/NeonSign/", "NeonSign");
}

void PC_NeonSign::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting PC_NeonSign static models...\n");
    #endif

    Entity::deleteModels(&PC_NeonSign::models);
}
