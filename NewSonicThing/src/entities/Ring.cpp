#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "ring.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/camera.h"
#include "../entities/playersonic.h"
#include "../collision/collisionchecker.h"
#include "../collision/triangle3d.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particlemaster.h"
#include "shieldmagnet.h"
#include "ringmoving.h"

#include <list>
#include <iostream>
#include <algorithm>

std::list<TexturedModel*> Ring::models;


Ring::Ring()
{

}

Ring::Ring(float x, float y, float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    moves = false;
    xVel = 0;
    yVel = 0;
    zVel = 0;
    grabTimer = 0;
    givesPoints = true;
    updateTransformationMatrix();

    //Vector3f norm(0, 5, 0);
    //if (CollisionChecker::checkCollision(position.x, position.y-1, position.z, position.x, position.y+1, position.z))
    //{
    //    norm = CollisionChecker::getCollideTriangle()->normal.scaleCopy(5.0f);
    //}
    //else if (CollisionChecker::checkCollision(position.x-1, position.y, position.z, position.x+1, position.y, position.z))
    //{
    //    norm = CollisionChecker::getCollideTriangle()->normal.scaleCopy(5.0f);
    //}
    //else if (CollisionChecker::checkCollision(position.x, position.y, position.z-1, position.x, position.y, position.z+1))
    //{
    //    norm = CollisionChecker::getCollideTriangle()->normal.scaleCopy(5.0f);
    //}

    //Vector3f res = position + norm;
    //printf("v %f %f %f\n", res.x, res.y, res.z);

    //position = res;
}

Ring::Ring(float x, float y, float z, float xVel, float yVel, float zVel)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    moves = true;
    this->xVel = xVel;
    this->yVel = yVel;
    this->zVel = zVel;
    grabTimer = 60;
    givesPoints = false;
    updateTransformationMatrix();
}

void Ring::step()
{
    extern float dt;
    increaseRotation(0, 5*60*dt, 0);
    updateTransformationMatrix();

    if (fabsf(position.y - Global::gameMainPlayer->position.y) < 40 &&
        fabsf(position.z - Global::gameMainPlayer->position.z) < 40 &&
        fabsf(position.x - Global::gameMainPlayer->position.x) < 40 &&
        grabTimer == 0)
    {
        float toPlayerDistSquared = (Global::gameMainPlayer->getCenterPosition() - position).lengthSquared();
        if (toPlayerDistSquared < (11.0f*11.0f)) //collect ring range
        {
            AudioPlayer::play(4, &position);

            for (int i = 0; i < 10; i++)
            {
                Vector3f pos(
                    position.x + Maths::random() * 8 - 4,
                    position.y + Maths::random() * 8 - 4,
                    position.z + Maths::random() * 8 - 4);
            
                Vector3f partVel(0, 0.4f*60, 0);
            
                ParticleMaster::createParticle(ParticleResources::textureSparkleYellow, &pos, &partVel,
                    0.025f*60*60, 0.5f, 0, 4, -(4.0f / 0.5f), false, false, 0.5f, true);
            }

            Global::increaseRingCount(1);

            if (givesPoints)
            {
                Global::gameScore += 10;
            }

            Global::deleteChunkedEntity(this);
            return;
        }
        else
        {
            if (Global::gameMainPlayer->getShieldMagnet() != nullptr)
            {
                if (toPlayerDistSquared < 36.0f*36.0f) //attract ring range
                {
                    Global::deleteChunkedEntity(this);
                    RingMoving* newRing = new RingMoving(position.x, position.y, position.z); INCR_NEW("RingMoving")
                    Global::addEntity(newRing);
                }
            }
        }
    }
}

std::list<TexturedModel*>* Ring::getModels()
{
    return &Ring::models;
}

void Ring::loadStaticModels()
{
    if (Ring::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading ring static models...\n");
    #endif

    ObjLoader::loadModel(&Ring::models, "res/Models/Objects/Ring/", "Ring");
}

void Ring::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting ring static models...\n");
    #endif

    Entity::deleteModels(&Ring::models);
}

const inline bool Ring::canLightdashOn()
{
    return true;
}
