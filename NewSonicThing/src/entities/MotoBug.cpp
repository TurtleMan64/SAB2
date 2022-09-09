#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "motobug.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"
#include "../collision/collisionchecker.hpp"
#include "../collision/triangle3d.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> MotoBug::models;

MotoBug::MotoBug()
{

}

MotoBug::MotoBug(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;

    Vector3f yAxis(0, 1, 0);
    lookDir.set(1, 0, 0);
    lookDir = Maths::rotatePoint(&lookDir, &yAxis, Maths::random()*Maths::PI*2);
    rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));

    updateTransformationMatrix();
}

void MotoBug::step()
{
    extern float dt;

    if (std::abs(position.x - Global::gameMainPlayer->position.x) < MotoBug::moveRange &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < MotoBug::moveRange &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < MotoBug::moveRange)
    {
        Vector3f toPlayerDiff = Global::gameMainPlayer->position - position;

        if (playerIsInHitbox())
        {
            Vector3f center = getHomingCenter();
            if (Global::gameMainPlayer->isVulnerable())
            {
                Global::gameMainPlayer->takeDamage(&center);
            }
            else if (Global::gameMainPlayer->canDealDamage())
            {
                die();
                Global::gameMainPlayer->rebound(&center);
                Global::gameMainPlayer->increaseCombo();
                return;
            }
        }

        //rotate toward player
        toPlayerDiff.y = 0;
        toPlayerDiff.normalize();
        lookDir = Maths::interpolateVector(&lookDir, &toPlayerDiff, MotoBug::lookSpeed*dt);

        clock += dt;
        furyTimer = std::fmaxf(0, furyTimer - dt);
        yVel = std::fmaxf(-120, yVel - (6*dt)*60);

        ControllablePlayer* p = Global::gameMainPlayer;
        
        float xDiff = p->position.x-position.x;
        float zDiff = p->position.z-position.z;
        float angleRad = Maths::toRadians(rotY);
        float newZ = ((xDiff)*-sinf(angleRad) - (zDiff)*cosf(angleRad));
        float mag = sqrtf((xDiff*xDiff)+(zDiff*zDiff));

        applyFriction(1.3f*60*dt);

        if (!inAir && triCol->normal.y >= 0.5f)
        {
            float pushUpValue = -position.y+(((-triCol->A*(position.x + xVel)) +
                                              (-triCol->C*(position.z + zVel)) -
                                               (triCol->D))/triCol->B);
            yVel = pushUpValue;
        }

        if (mag < MotoBug::activationRange)
        {
            if (furyTimer == 0)
            {
                furyTimer = furyTimerMax;
                rotY = (Maths::toDegrees(atan2f(-zDiff, xDiff)));
                //AudioRes.playSound(27, 1, getPosition());
                xVel = 0;
                zVel = 0;
                yVel = 15.0f;
                position.y += 1;
            }
        }
        else
        {
            if (mag < MotoBug::moveRange && furyTimer == 0)
            {
                float ran = Maths::random();
                if (ran < 0.01f*60*dt)
                {
                    if (ran < 0.003f*60*dt && clock > 0.5f)
                    {
                        xVel = 0;
                        zVel = 0;
                        yVel = 1;
                        rotY = (Maths::random()*360);
                        clock = 0;
                    }
                    else
                    {
                        rotY += (Maths::random()*120-60)*dt*60;
                    }
                }
                
                if (clock > 0.5f && furyTimer == 0)
                {
                    moveMeSlow();
                }
            }
        }

        if (furyTimer > 0.5f)
        {
            if (newZ >= 2.8f)
            {
                rotY += 2.8f*dt*60;
            }
            else if (newZ <= -2.8f)
            {
                rotY += -2.8f*dt*60;
            }
            
            if (furyTimer < furyTimerMax - 1.05f)
            {
                moveMe();
            }
        }

        if (mag < MotoBug::moveRange)
        {
            if (CollisionChecker::checkCollision(position.x, position.y, position.z, position.x+xVel*dt, position.y+yVel*dt, position.z+zVel*dt) == false)
            {
                if (CollisionChecker::checkCollision(position.x+xVel*dt, position.y+yVel*dt, position.z+zVel*dt, position.x+xVel*dt, position.y + yVel*dt - 1.0f, position.z+zVel*dt) == true)
                {
                    Triangle3D* savedTri = CollisionChecker::getCollideTriangle();
                    Vector3f triColPosition(CollisionChecker::getCollidePosition());

                    float xTest =  10*cosf(Maths::toRadians(rotY));
                    float zTest = -10*sinf(Maths::toRadians(rotY));
                    if (CollisionChecker::checkCollision(position.x+xTest, position.y, position.z+zTest, position.x+xTest, position.y-10.0f, position.z+zTest) == true)
                    {
                        inAir = false;
                        triCol = savedTri;
                    
                        float colXNormal = triCol->normal.x;
                        float colYNormal = triCol->normal.y;
                        float colZNormal = triCol->normal.z;
                    
                        if (colYNormal >= 0.75f)
                        {
                            position.x = (triColPosition.x);
                            position.y = (triColPosition.y+colYNormal*0.5f);
                            position.z = (triColPosition.z);
                        }
                        else
                        {
                            if (colYNormal >= 0.5f)
                            {
                                position.x = (triColPosition.x);
                                position.y = (triColPosition.y+colYNormal*0.5f);
                                position.z = (triColPosition.z);
                                xVel+=colXNormal*0.05f*60*60*dt;
                                zVel+=colZNormal*0.05f*60*60*dt;
                            }
                            else
                            {
                                position.x = (triColPosition.x+colXNormal*0.5f);
                                position.y = (triColPosition.y+colYNormal*0.5f);
                                position.z = (triColPosition.z+colZNormal*0.5f);
                                xVel = 0;
                                zVel = 0;
                                rotY = (rotY+180);
                            }
                        }
                    }
                    else //turn around if we are abound to drive off a cliff
                    {
                        rotY = (rotY+180);
                    }
                }
                else
                {
                    if (inAir)
                    {
                        increasePosition(xVel*dt, yVel*dt, zVel*dt);
                    }
                    else if (furyTimer > 0)
                    {
                        inAir = true;
                        increasePosition(xVel*dt, yVel*dt, zVel*dt);
                    }
                    //else //if we are about to walk off a ledge
                    //{
                        //turn around 180? doesnt work well
                    //}
                }
            }
            else
            {
                inAir = false;
                triCol = CollisionChecker::getCollideTriangle();
                Vector3f* triColPosition = CollisionChecker::getCollidePosition();
                
                float colXNormal = triCol->normal.x;
                float colYNormal = triCol->normal.y;
                float colZNormal = triCol->normal.z;
                
                //yVel = 0;
                
                if (colYNormal >= 0.75f)
                {
                    position.x = (triColPosition->x);
                    position.y = (triColPosition->y+colYNormal*0.5f);
                    position.z = (triColPosition->z);
                }
                else
                {
                    if (colYNormal >= 0.5f)
                    {
                        position.x = (triColPosition->x);
                        position.y = (triColPosition->y+colYNormal*0.5f);
                        position.z = (triColPosition->z);
                        xVel+=colXNormal*0.05f*60*60*dt;
                        zVel+=colZNormal*0.05f*60*60*dt;
                    }
                    else
                    {
                        position.x = (triColPosition->x+colXNormal*0.5f);
                        position.y = (triColPosition->y+colYNormal*0.5f);
                        position.z = (triColPosition->z+colZNormal*0.5f);
                        xVel = 0;
                        zVel = 0;
                        rotY = (rotY+180);
                    }
                }
            }
        }

        if (position.y < Global::waterHeight - 10)
        {
            die();
        }

        Maths::createTransformationMatrix(&transformationMatrix, &position, 0, rotY, 0, 0, 1);
    }
}

void MotoBug::die()
{
    AudioPlayer::play(3, &position);

    Global::deleteChunkedEntity(this);

    visible = false;

    float height = 10.0f;
    float spread = 20.0f;

    Vector3f vel(0, 0, 0);
    Vector3f toCamDiff = Global::gameCamera->eye - position;
    toCamDiff.setLength(20);
    
    for (int i = 7; i != 0; i--)
    {
        Vector3f pos(
            position.x + spread*(Maths::random() - 0.5f),
            position.y + spread*(Maths::random() - 0.5f) + height,
            position.z + spread*(Maths::random() - 0.5f));
        pos = pos + toCamDiff; //so that these aren't behind the big explosion

        ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &vel, 0, 0.75f, 3*Maths::random() + 6, 0, false, false, 0.5f, true);
    }
    
    Vector3f pos(
        position.x,
        position.y + height,
        position.z);
    
    ParticleMaster::createParticle(ParticleResources::textureExplosion2, &pos, &vel, 0, 0.916f, 20, 0, false, false, 0.75f, true);
    
    Global::gameScore += 100;
}

bool MotoBug::playerIsInHitbox()
{
    Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;
    float diffY = diff.y;
    diff.y = 0;
    float circleDistSquared = diff.lengthSquared();

    return (circleDistSquared <= MotoBug::hitboxRadius*MotoBug::hitboxRadius &&
        diffY >= 0 &&
        diffY <= MotoBug::hitboxHeight);
}

void MotoBug::moveMe()
{
    xVel = 150*cosf(Maths::toRadians(-rotY));
    zVel = 150*sinf(Maths::toRadians(-rotY));
}
    
void MotoBug::moveMeSlow()
{
    xVel = 15*cosf(Maths::toRadians(-rotY));
    zVel = 15*sinf(Maths::toRadians(-rotY));
}
    
void MotoBug::applyFriction(float /*frictionToApply*/)
{
    xVel = 0;
    zVel = 0;
}

std::list<TexturedModel*>* MotoBug::getModels()
{
    return &MotoBug::models;
}

void MotoBug::loadStaticModels()
{
    if (MotoBug::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading MotoBug static models...\n");
    #endif

    ModelLoader::loadModel(&MotoBug::models, "res/Models/Objects/MotoBug/", "MotoBug");
}

void MotoBug::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting MotoBug static models...\n");
    #endif

    Entity::deleteModels(&MotoBug::models);
}

bool MotoBug::canHomingAttackOn() const
{
    return true;
}

Vector3f MotoBug::getHomingCenter() const
{
    return Vector3f(position.x, position.y+11, position.z);
}

bool MotoBug::isEnemy() const
{
    return true;
}
