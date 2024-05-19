#include "pcflame.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../toolbox/vector.hpp"
#include "../camera.hpp"
#include "../../audio/source.hpp"
#include "../../audio/audioplayer.hpp"

#include <vector>

std::vector<TexturedModel*> PC_Flame::models;

PC_Flame::PC_Flame(float x, float y, float z, float s)
{
    position.x = x;
    position.y = y;
    position.z = z;

    scale = s;

    updateTransformationMatrix();
}

PC_Flame::~PC_Flame()
{
    if (sourceFire != nullptr)
    {
        sourceFire->stop();
        sourceFire = nullptr;
    }
}

void PC_Flame::step()
{
    Vector3f diff = Global::gameCamera->target - Global::gameCamera->eye;
    //Vector3f diff = position - Global::gameCamera->eye;

    // Face the camera
    Maths::sphereAnglesFromPosition(&diff, &rotY, &rotZ);
    rotZ = Maths::clamp(-45.0f, rotZ, 45.0f);
    transformationMatrix.setIdentity();
    transformationMatrix.translate(&position);
    transformationMatrix.rotate(Maths::toRadians(rotY), &Y_AXIS);
    transformationMatrix.rotate(Maths::toRadians(rotZ), &Z_AXIS);
    transformationMatrix.scale(scale);

    // Play fire crackle sound, but only for big flames
    if ((position - Global::gameCamera->eye).lengthSquared() < 300*300 && scale >= 94)
    {
        if (sourceFire == nullptr)
        {
            sourceFire = AudioPlayer::play(80, &position, 1.0f + 0.2f*Maths::random(), true);
            sourceFire->setSoundCompletion(Maths::random());
        }
    }
    else
    {
        if (sourceFire != nullptr)
        {
            sourceFire->stop();
            sourceFire = nullptr;
        }
    }
}

std::vector<TexturedModel*>* PC_Flame::getModels()
{
    return &PC_Flame::models;
}

void PC_Flame::loadStaticModels()
{
    if (PC_Flame::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading PC_Flame static models...\n");
    #endif

    ModelLoader::loadModel(&PC_Flame::models, "res/Models/Levels/PyramidCave/Flame/", "Flame");
}

void PC_Flame::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting PC_Flame static models...\n");
    #endif

    Entity::deleteModels(&PC_Flame::models);
}
