#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "jumpramp.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../collision/collisionchecker.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> JumpRamp::models;

JumpRamp::JumpRamp()
{

}

JumpRamp::JumpRamp(float x, float y, float z, float yRot)
{
    position.x = x;
    position.y = y;
    position.z = z;
    scale = 1;
    renderOrder = 3;
    visible = true;
    playerIsIn = false;

    rotX = 0;
    rotY = yRot;
    rotZ = 0;
    rotRoll = 0;
    updateTransformationMatrix();
}

void JumpRamp::step()
{
    if (abs(getX() - Global::gameCamera->eye.x) > 1000)
    {
        setVisible(false);
    }
    else
    {
        if (abs(getZ() - Global::gameCamera->eye.z) > 1000)
        {
            setVisible(false);
        }
        else
        {
            setVisible(true);

            if (abs(getX() - Global::gameMainPlayer->getPosition()->x) < 10 &&
                abs(getZ() - Global::gameMainPlayer->getPosition()->z) < 10 &&
                abs(getY() - Global::gameMainPlayer->getPosition()->y) < 10)
            {
                Vector3f diff(
                    getX()-Global::gameMainPlayer->getPosition()->x, 
                    getY()-Global::gameMainPlayer->getPosition()->y,
                    getZ()-Global::gameMainPlayer->getPosition()->z);

                if (diff.lengthSquared() < (8.0f*8.0f))
                {
                    if (!playerIsIn)
                    {
                        AudioPlayer::play(0, getPosition());
                        //Global::gameMainVehicle->giveMeAJump();
                    }

                    playerIsIn = true;
                }
                else
                {
                    playerIsIn = false;
                }
            }
            else
            {
                playerIsIn = false;
            }
        }
    }
}

std::list<TexturedModel*>* JumpRamp::getModels()
{
    return &JumpRamp::models;
}

void JumpRamp::loadStaticModels()
{
    if (JumpRamp::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading JumpRamp static models...\n");
    #endif

    loadModel(&JumpRamp::models,  "res/Models/Misc/JumpRamp/", "JumpRamp");
}

void JumpRamp::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting JumpRamp static models...\n");
    #endif

    Entity::deleteModels(&JumpRamp::models);
}
