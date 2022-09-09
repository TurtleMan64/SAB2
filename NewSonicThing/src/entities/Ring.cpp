#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "ring.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../entities/playersonic.hpp"
#include "../collision/collisionchecker.hpp"
#include "../collision/triangle3d.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particlemaster.hpp"
#include "shieldmagnet.hpp"
#include "ringmoving.hpp"
#include "../toolbox/matrix.hpp"

#include <list>
#include <iostream>

std::list<TexturedModel*> Ring::models;


Ring::Ring()
{

}

Ring::Ring(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
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

    Global::gameDebugRingCount+=1;
}

Ring::Ring(float x, float y, float z, float xVel, float yVel, float zVel)
{
    position.x = x;
    position.y = y;
    position.z = z;
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
    transformationMatrix.setIdentity();
    transformationMatrix.translate(&position);
    transformationMatrix.rotate(Maths::toRadians(Global::gameClock*300), &Y_AXIS);

    if (std::abs(position.y - Global::gameMainPlayer->position.y) < 40 &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < 40 &&
        std::abs(position.x - Global::gameMainPlayer->position.x) < 40 &&
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
                    0.025f*60*60, 0.5f, 4, -(4.0f / 0.5f), false, false, 0.5f, true);
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
    printf("Loading Ring static models...\n");
    #endif

    ModelLoader::loadModel(&Ring::models, "res/Models/Objects/Ring/", "Ring");
}

void Ring::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Ring static models...\n");
    #endif

    Entity::deleteModels(&Ring::models);
}

inline bool Ring::canLightdashOn() const
{
    return true;
}
