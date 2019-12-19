#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "checkpoint.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "dummy.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../guis/guimanager.h"
#include "../collision/collisionchecker.h"
#include "../menu/timer.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Checkpoint::modelsBase;
std::list<TexturedModel*> Checkpoint::modelsBall;

float Checkpoint::colHorizontal = 13;
float Checkpoint::colVertical   = 9;
ALuint Checkpoint::savedBGMIntro = AL_NONE;
ALuint Checkpoint::savedBGMLoop  = AL_NONE;

Checkpoint::Checkpoint()
{

}

Checkpoint::Checkpoint(float x, float y, float z, float yRot)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = yRot;
    rotZ = 0;
    scale = 1;
    visible = true;
    isHit = false;
    updateTransformationMatrix();

    if (Global::spawnAtCheckpoint)
    {
        Vector3f diff = Global::checkpointPlayerPos - position;

        if (diff.length() < 50)
        {
            isHit = true;
            if (Checkpoint::savedBGMIntro != AL_NONE)
            {
                AudioPlayer::playBGMWithIntro(Checkpoint::savedBGMIntro, Checkpoint::savedBGMLoop);
            }
            else
            {
                AudioPlayer::playBGM(Checkpoint::savedBGMLoop);
            }
        }
    }

    float rot = Maths::toRadians(rotY);

    Vector3f off(0, 8.6f, -15.12f);
    float newX =   off.x*cosf(rot) - off.z*sinf(rot);
    float newZ = -(off.z*cosf(rot) + off.x*sinf(rot));
    Vector3f ball1Pos(newX, off.y, newZ);
    ball1Pos = ball1Pos + position;

    off.set(0, 8.6f, 15.12f);
    newX =   off.x*cosf(rot) - off.z*sinf(rot);
    newZ = -(off.z*cosf(rot) + off.x*sinf(rot));
    Vector3f ball2Pos(newX, off.y, newZ);
    ball2Pos = ball2Pos + position;

    ball1 = new Dummy(&Checkpoint::modelsBall); INCR_NEW("Entity")
    Main_addChunkedEntity(ball1);
    ball1->setVisible(true);
    ball1->setPosition(&ball1Pos);
    ball1->setRotY(rotY+90);
    ball1->updateTransformationMatrix();

    ball2 = new Dummy(&Checkpoint::modelsBall); INCR_NEW("Entity")
    Main_addChunkedEntity(ball2);
    ball2->setVisible(true);
    ball2->setPosition(&ball2Pos);
    ball2->setRotY(rotY-90);
    ball2->updateTransformationMatrix();

    if (isHit)
    {
        ball1->setRotZ(90);
        ball2->setRotZ(90);
        ball1->updateTransformationMatrix();
        ball2->updateTransformationMatrix();
        ball1->baseColour.set(1, 0, 0);
        ball2->baseColour.set(1, 0, 0);
    }
}

void Checkpoint::step()
{
    if (Global::gameIsRaceMode)
    {
        Main_deleteChunkedEntity(this);
        Main_deleteChunkedEntity(ball1);
        Main_deleteChunkedEntity(ball2);
        return;
    }

    const float playerColH = 4.0f;
    const float playerColV = 4.0f;

    if (fabsf(position.y - Global::gameMainPlayer->position.y) < 40 &&
        fabsf(position.z - Global::gameMainPlayer->position.z) < 40 &&
        fabsf(position.x - Global::gameMainPlayer->position.x) < 40)
    {
        if (!isHit &&
            Global::gameMainPlayer->getX() > position.x - colHorizontal - playerColH && Global::gameMainPlayer->getX() < position.x + colHorizontal + playerColH &&
            Global::gameMainPlayer->getZ() > position.z - colHorizontal - playerColH && Global::gameMainPlayer->getZ() < position.z + colHorizontal + playerColH &&
            Global::gameMainPlayer->getY() > position.y - colVertical   - playerColV && Global::gameMainPlayer->getY() < position.y + colVertical   + playerColV)
        {
            AudioPlayer::play(45, getPosition());

            Checkpoint::savedBGMIntro = AudioPlayer::bgmIntro;
            Checkpoint::savedBGMLoop  = AudioPlayer::bgmLoop;

            Vector3f playerDir(&Global::gameMainPlayer->vel);
            playerDir.y = 0;
            playerDir.setLength(0.001f);

            isHit = true;
            Global::spawnAtCheckpoint  = true;

            Global::checkpointPlayerPos.set(&Global::gameMainPlayer->position);
            Global::checkpointPlayerDir.set(&playerDir);
            Global::checkpointCamDir.set(&Global::gameMainPlayer->camDir);

            if (Global::mainHudTimer != nullptr)
            {
                Global::checkpointTime = Global::mainHudTimer->totalTime;
            }

            if (!CollisionChecker::checkCollision(
                Global::checkpointPlayerPos.x, Global::checkpointPlayerPos.y,    Global::checkpointPlayerPos.z,
                Global::checkpointPlayerPos.x, Global::checkpointPlayerPos.y-30, Global::checkpointPlayerPos.z))
            {
                Global::checkpointPlayerPos = position;
                Global::checkpointPlayerPos.y += 5;
            }

            ball1->setRotZ(90);
            ball2->setRotZ(90);
            ball1->updateTransformationMatrix();
            ball2->updateTransformationMatrix();
            ball1->baseColour.set(1, 0, 0);
            ball2->baseColour.set(1, 0, 0);
        }
    }
}

std::list<TexturedModel*>* Checkpoint::getModels()
{
    return &Checkpoint::modelsBase;
}

void Checkpoint::loadStaticModels()
{
    if (Checkpoint::modelsBase.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading Checkpoint static models...\n");
    #endif

    loadModel(&Checkpoint::modelsBase, "res/Models/Objects/Checkpoint/", "Base");
    loadModel(&Checkpoint::modelsBall, "res/Models/Objects/Checkpoint/", "Ball");
}

void Checkpoint::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting Checkpoint static models...\n");
    #endif

    Entity::deleteModels(&Checkpoint::modelsBase);
    Entity::deleteModels(&Checkpoint::modelsBall);
}
