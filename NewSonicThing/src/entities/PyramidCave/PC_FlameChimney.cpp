#include "pcflamechimney.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../toolbox/vector.hpp"
#include "../camera.hpp"

#include <vector>

std::vector<TexturedModel*> PC_FlameChimney::models;

PC_FlameChimney::PC_FlameChimney(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;

    scale = 32;

    visible = false;

    animationTime = 0.0f;
    waitTime = 2*Maths::random() + 1.0f;

    renderTextureAnimationOverride = 0.9999f;

    updateTransformationMatrix();
}

void PC_FlameChimney::step()
{
    extern float dt;

    visible = false;
    renderTextureAnimationOverride = -1.0f;

    if (animationTime > 0.0f)
    {
        visible = true;
        renderTextureAnimationOverride = (1.0f - animationTime)/1.2f;

        Vector3f diff = Global::gameCamera->target - Global::gameCamera->eye;

        // Face the camera
        Maths::sphereAnglesFromPosition(&diff, &rotY, &rotZ);
        transformationMatrix.setIdentity();
        transformationMatrix.translate(&position);
        transformationMatrix.rotate(Maths::toRadians(rotY), &Y_AXIS);
        transformationMatrix.rotate(Maths::toRadians(rotZ), &Z_AXIS);
        transformationMatrix.scale(scale);

        animationTime -= dt;

        if (animationTime <= 0.0f)
        {
            waitTime = 2*Maths::random() + 1.0f;
        }
    }

    if (waitTime > 0.0f)
    {
        visible = false;

        waitTime -= dt;

        if (waitTime <= 0.0f)
        {
            animationTime = 1.0f;
        }
    }
}

std::vector<TexturedModel*>* PC_FlameChimney::getModels()
{
    return &PC_FlameChimney::models;
}

void PC_FlameChimney::loadStaticModels()
{
    if (PC_FlameChimney::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading PC_FlameChimney static models...\n");
    #endif

    ModelLoader::loadModel(&PC_FlameChimney::models, "res/Models/Levels/PyramidCave/FlameChimney/", "FlameChimney");
}

void PC_FlameChimney::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting PC_FlameChimney static models...\n");
    #endif

    Entity::deleteModels(&PC_FlameChimney::models);
}
