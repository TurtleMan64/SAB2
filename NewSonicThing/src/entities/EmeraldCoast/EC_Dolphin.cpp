#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "ecdolphin.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../entities/playersonic.h"
#include "../../toolbox/maths.h"
#include "../camera.h"
#include "../../audio/audioplayer.h"
#include "../../particles/particle.h"
#include "../../particles/particleresources.h"
#include "../../particles/particlemaster.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> EC_Dolphin::models;

EC_Dolphin::EC_Dolphin()
{
    
}

EC_Dolphin::EC_Dolphin(
    float x, float y, float z, 
    float dx, float dz,
    float triggerX, float triggerY, float triggerZ,
    float triggerRadius)
{
    position.x = x + 30*Maths::random();
    position.y = y;
    position.z = z + 30*Maths::random();
    hSpd = 0.0f;
    yVel = 0.0f;
    rotY = Maths::toDegrees(atan2f(-dz, dx));
    visible = false;
    inWaterPrevious = true;
    triggerTimer = -1.0f;
    this->triggerX = triggerX + 10*Maths::random();
    this->triggerY = triggerY + 10*Maths::random();
    this->triggerZ = triggerZ + 10*Maths::random();
    this->triggerRadius = triggerRadius;
    updateTransformationMatrix();
}

extern float dt;

void EC_Dolphin::step()
{
    if (triggerTimer < 0.0f)
    {
        visible = false;

        float xDiff = Global::gameMainPlayer->position.x - triggerX;
        float yDiff = Global::gameMainPlayer->position.y - triggerY;
        float zDiff = Global::gameMainPlayer->position.z - triggerZ;

        float dist = (xDiff*xDiff + yDiff*yDiff + zDiff*zDiff);

        if (dist < triggerRadius*triggerRadius)
        {
            triggerTimer = 0.0f;
            visible = true;
            hSpd = 5.0f*60;
        }
    }
    else
    {
        triggerTimer += dt;

        float xDiff =  hSpd*cosf(Maths::toRadians(rotY))*dt;
        float zDiff = -hSpd*sinf(Maths::toRadians(rotY))*dt;
        
        if (position.y > 0.0f)
        {
            yVel -= 0.2f*60*60*dt;
            hSpd = fmaxf(hSpd - 0.01f*60*dt, 3*60);

            if (inWaterPrevious)
            {
                AudioPlayer::play(5, &position);
                Vector3f pos(position.x, 5, position.z);
                Vector3f velZero(0, 0, 0);
                ParticleMaster::createParticle(ParticleResources::textureSplash, &pos, &velZero, 0, 0.5f, 0, 10, 0, false, false, 1.0f, true);
            }

            inWaterPrevious = false;
        }
        else
        {
            if (triggerTimer < 1.5f)
            {
                yVel += 0.75f*60*60*dt;
                hSpd = fminf(hSpd + 0.05f*60*dt, 5*60);
            }

            if (!inWaterPrevious)
            {
                AudioPlayer::play(5, &position);
                Vector3f pos(position.x, 5, position.z);
                Vector3f velZero(0, 0, 0);
                ParticleMaster::createParticle(ParticleResources::textureSplash, &pos, &velZero, 0, 0.5f, 0, 10, 0, false, false, 1.0f, true);
            }

            inWaterPrevious = true;
        }

        if (position.y < -150.0f)
        {
            Global::deleteChunkedEntity(this);
        }

        if (yVel > 10.0f*60)
        {
            yVel = 10.0f*60;
        }

        if (yVel < -10.0f*60)
        {
            yVel = -10.0f*60;
        }

        rotZ = Maths::toDegrees(atan2f(yVel*dt, hSpd*dt));

        increasePosition(xDiff, yVel*dt, zDiff);
        updateTransformationMatrix();
    }
}

std::list<TexturedModel*>* EC_Dolphin::getModels()
{
    return &EC_Dolphin::models;
}

void EC_Dolphin::loadStaticModels()
{
    if (EC_Dolphin::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading EC_Dolphin static models...\n");
    #endif

    ObjLoader::loadModel(&EC_Dolphin::models, "res/Models/Levels/EmeraldCoast/Dolphin/", "Dolphin");
}

void EC_Dolphin::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting EC_Dolphin static models...\n");
    #endif

    Entity::deleteModels(&EC_Dolphin::models);
}
