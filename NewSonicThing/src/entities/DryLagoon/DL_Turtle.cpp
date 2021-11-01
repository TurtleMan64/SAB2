#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "dlturtle.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../toolbox/maths.h"
#include "../camera.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"
#include "../controllableplayer.h"
#include "../../particles/particle.h"
#include "../../particles/particleresources.h"
#include "../../particles/particlemaster.h"
#include "../../toolbox/input.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> DL_Turtle::models;
CollisionModel* DL_Turtle::cmOriginal;

DL_Turtle::DL_Turtle()
{
    
}

DL_Turtle::DL_Turtle(
    float x, float y, float z, float rotY, 
    float xt, float yt, float zt, 
    float camX, float camY, float camZ)
{
    position.x = x;
    position.y = y;
    position.z = z;
    this->rotY = rotY;
    updateTransformationMatrix();

    target.set(xt, yt, zt);
    targetCam.set(camX, camY, camZ);
    targetCam.normalize();

    collideModelOriginal = DL_Turtle::cmOriginal;
    collideModelTransformed = DL_Turtle::cmOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void DL_Turtle::step()
{
    extern float dt;

    if (collideModelTransformed->playerIsOn)
    {
        //if (Global::gameMainPlayer->position.y >= position.y + 15.5f)
        {
            if ((Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2) ||
                (Input::inputs.INPUT_ACTION3 && !Input::inputs.INPUT_PREVIOUS_ACTION3))
            {
                warpTimer = warpTimerMax;
                Global::gameMainPlayer->canMoveTimer = warpTimerMax*2 + 0.16666f;

                Vector3f vel(0, 0, 0);
                ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, &Global::gameCamera->fadePosition1, &vel, 0, warpTimerMax*2 + 0.16666f, 0, 400, 0, true, false, 1.0f, false);
            }
        }
    }

    if (warpTimer > 0.0f)
    {
        warpTimer -= dt;
        Global::gameMainPlayer->vel.set(0, 0.0001f, 0);
        Global::gameMainPlayer->position.set(position.x, position.y + 20.0f, position.z);
        Global::gameMainPlayer->relativeUp.set(0, 1, 0);

        if (warpTimer <= 0.0f)
        {
            Global::gameMainPlayer->vel = targetCam.scaleCopy(0.00001f);
            Global::gameMainPlayer->position = target;

            Global::gameMainPlayer->camDir = targetCam;
            Global::gameMainPlayer->camDirSmooth = targetCam;
        }
    }
}

std::list<TexturedModel*>* DL_Turtle::getModels()
{
    return &DL_Turtle::models;
}

void DL_Turtle::loadStaticModels()
{
    if (DL_Turtle::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading DL_Turtle static models...\n");
    #endif

    ObjLoader::loadModel(&DL_Turtle::models, "res/Models/Levels/DryLagoon/Objects/", "Turtle");

    if (DL_Turtle::cmOriginal == nullptr)
    {
        DL_Turtle::cmOriginal = ObjLoader::loadCollisionModel("Models/Levels/DryLagoon/Objects/", "TurtleCollision");
    }
}

void DL_Turtle::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting DL_Turtle static models...\n");
    #endif

    Entity::deleteModels(&DL_Turtle::models);
    Entity::deleteCollisionModel(&DL_Turtle::cmOriginal);
}
