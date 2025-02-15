#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particlemaster.hpp"
#include "hintmonitor.hpp"
#include "emeraldmanager.hpp"
#include "emeraldpiece.hpp"
#include "../toolbox/input.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> HintMonitor::models;

HintMonitor::HintMonitor()
{

}

HintMonitor::HintMonitor(float x, float y, float z, float rotationY)
{
    rotY = rotationY;
    position.set(x, y, z);
    visible = true;
    updateTransformationMatrix();

    if (!Global::gameIsNormalMode)
    {
        Global::deleteEntity(this);
    }
}

void HintMonitor::step()
{
    extern float dt;

    if (readingTimer > 0.0f)
    {
        readingTimer += dt;

        if (readingTimer > 1.0f &&
            ((Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2) ||
             (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1)))
        {
            readingTimer = 0.0f;
            fadeoutTimer = 1.0f;
            Global::gameMainPlayer->canMoveTimer = 1.0f;
            Vector3f vel(0,0,0);
            ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, &Global::gameCamera->fadePosition1, &vel, 0, 1.0f, 400, 0, true, false, 1.0f, false);
        }

        if (readingTimer > 0.5f)
        {
            Global::gameCamera->eye = hintEyePos;
            Global::gameCamera->target = hintPiecePos;
            Global::gameCamera->up.set(0, 1, 0);
        }

        Global::gameMainPlayer->position = storedPlayerPos;
        Global::gameMainPlayer->vel = (position - storedPlayerPos).scaleCopy(0.0001f);
    }

    if (fadeoutTimer > 0.0f)
    {
        fadeoutTimer -= dt;

        if (fadeoutTimer > 0.5f)
        {
            Global::gameCamera->eye = hintEyePos;
            Global::gameCamera->target = hintPiecePos;
            Global::gameCamera->up.set(0, 1, 0);
        }
        else
        {
            visible = false;
        }

        Global::gameMainPlayer->position = storedPlayerPos;
        Global::gameMainPlayer->vel = (position - storedPlayerPos).scaleCopy(0.0001f);

        if (fadeoutTimer <= 0.0f)
        {
            Global::gameMainPlayer->setReadingHint(false);
        }
    }

    if (!collected)
    {
        if (std::abs(position.y - (Global::gameMainPlayer->position.y + 30)) < 30 &&
            std::abs(position.z - Global::gameMainPlayer->position.z) < 40 &&
            std::abs(position.x - Global::gameMainPlayer->position.x) < 40)
        {
            Vector3f diff = Global::gameMainPlayer->position - position;
            diff.y = 0;
            diff.setLength(1.0f);

            Vector3f dir = Maths::spherePositionFromAngles(Maths::toRadians(-rotY), 0, 1.0f);
            Vector3f newdir = Maths::interpolateVector(&dir, &diff, 10*dt);

            rotY = Maths::toDegrees(atan2f(-newdir.z, newdir.x));

            if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2 &&
                Global::gameMainPlayer->canReadHint() && readingTimer == 0.0f)
            {
                diff = Global::gameMainPlayer->position - position;
                diff.y = 0;
                if (diff.lengthSquared() < (35.0f*35.0f))
                {
                    EmeraldPiece* closestPiece = EmeraldManager::getClosestPiece();

                    if (closestPiece != nullptr)
                    {
                        hintEyePos = closestPiece->getHintLocation();
                        hintPiecePos = closestPiece->hintCameraCenter;

                        storedPlayerPos = Global::gameMainPlayer->position;

                        readingTimer = 0.01f;

                        collected = true;
                    
                        Global::gameMainPlayer->vel = (position - storedPlayerPos).scaleCopy(0.0001f);
                        Global::gameMainPlayer->canMoveTimer = 100000000000.0f;
                        Global::gameMainPlayer->setReadingHint(true);
                        Vector3f vel(0,0,0);
                        ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, &Global::gameCamera->fadePosition1, &vel, 0, 1.0f, 400, 0, true, false, 1.0f, false);
                    }
                }
            }

            updateTransformationMatrix();
        }
    }
}

std::vector<TexturedModel*>* HintMonitor::getModels()
{
    return &HintMonitor::models;
}

void HintMonitor::loadStaticModels()
{
    if (HintMonitor::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading HintMonitor static models...\n");
    #endif

    ModelLoader::loadModel(&HintMonitor::models, "res/Models/Objects/Monitor/", "Monitor");
}

void HintMonitor::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting HintMonitor static models...\n");
    #endif

    Entity::deleteModels(&HintMonitor::models);
}
