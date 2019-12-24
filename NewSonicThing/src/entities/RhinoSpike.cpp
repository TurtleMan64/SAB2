#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "rhinospike.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../particles/particlemaster.h"
#include "../collision/collisionchecker.h"
#include "../collision/triangle3d.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> RhinoSpike::models;

RhinoSpike::RhinoSpike()
{

}

RhinoSpike::RhinoSpike(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 1;
    visible = true;

    Vector3f yAxis(0, 1, 0);
    lookDir.set(1, 0, 0);
    lookDir = Maths::rotatePoint(&lookDir, &yAxis, Maths::random()*Maths::PI*2);
    rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));

    updateTransformationMatrix();

    //baseColour.set(Maths::nextUniform()*5, Maths::nextUniform()*5, Maths::nextUniform()*5);
}

void RhinoSpike::step()
{
    extern float dt;

    if (fabsf(position.x - Global::gameMainPlayer->position.x) < RhinoSpike::moveRange &&
        fabsf(position.y - Global::gameMainPlayer->position.y) < RhinoSpike::moveRange &&
        fabsf(position.z - Global::gameMainPlayer->position.z) < RhinoSpike::moveRange)
    {
        Vector3f toPlayerDiff = Global::gameMainPlayer->position - position;

        if (playerIsInHitbox())
        {
            if (Global::gameMainPlayer->isVulnerable())
            {
                Global::gameMainPlayer->takeDamage(&position);
            }
            else if (Global::gameMainPlayer->canDealDamage())
            {
                die();
                Vector3f homingCenter(&position);
                homingCenter.y += 13.2651f;
                Global::gameMainPlayer->rebound(&homingCenter);
                Global::gameMainPlayer->increaseCombo();
            }
        }
        else if (playerIsInHurtbox())
        {
            if (Global::gameMainPlayer->canDealDamage())
            {
                Global::gameMainPlayer->takeDamage(&position);
            }
        }

        //rotate toward player
        toPlayerDiff.y = 0;
        toPlayerDiff.normalize();
        lookDir = Maths::interpolateVector(&lookDir, &toPlayerDiff, RhinoSpike::lookSpeed*dt);
        //rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));

        clock += dt;
        furyTimer = std::fmaxf(0, furyTimer - dt);
        yVel = std::fmaxf(-120, yVel - (6*dt)*60);

        ControllablePlayer* p = Global::gameMainPlayer;
        
        float xDiff = p->getX()-getX();
        float zDiff = p->getZ()-getZ();
        float angleRad = Maths::toRadians(getRotY());
        float newZ = ((xDiff)*-sinf(angleRad) - (zDiff)*cosf(angleRad));
        float mag = sqrtf((xDiff*xDiff)+(zDiff*zDiff));

        applyFriction(1.3f*60*dt);

        if (!inAir && triCol->normal.y >= 0.5f)
        {
            float pushUpValue = -getY()+(((-triCol->A*(getX()+xVel))+
                                          (-triCol->C*(getZ()+zVel))-
                                           (triCol->D))/triCol->B);
            yVel = pushUpValue;
        }

        if (mag < RhinoSpike::activationRange)
        {
            if (furyTimer == 0)
            {
                furyTimer = furyTimerMax;
                setRotY(Maths::toDegrees(atan2f(-zDiff, xDiff)));
                //AudioRes.playSound(27, 1, getPosition());
                xVel = 0;
                zVel = 0;
                yVel = 15.0f;
                increasePosition(0, 1, 0);
            }
        }
        else
        {
            if (mag < RhinoSpike::moveRange && furyTimer == 0)
            {
                float ran = Maths::random();
                if (ran < 0.01f*60*dt)
                {
                    if (ran < 0.003f*60*dt && clock > 0.5f)
                    {
                        xVel = 0;
                        zVel = 0;
                        yVel = 1;
                        setRotY(Maths::random()*360);
                        clock = 0;
                    }
                    else
                    {
                        increaseRotation(0, (Maths::random()*120-60)*dt*60, 0);
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
                increaseRotation(0, 2.8f*dt*60, 0);
            }
            else if (newZ <= -2.8f)
            {
                increaseRotation(0, -2.8f*dt*60, 0);
            }
            
            if (furyTimer < furyTimerMax - 1.05f)
            {
                moveMe();
            }
        }

        if (mag < RhinoSpike::moveRange)
        {
            if (CollisionChecker::checkCollision(getX(), getY(), getZ(), getX()+xVel*dt, getY()+yVel*dt, getZ()+zVel*dt) == false)
            {
                if (CollisionChecker::checkCollision(getX()+xVel*dt, getY()+yVel*dt, getZ()+zVel*dt, getX()+xVel*dt, getY() + yVel*dt - 1.0f, getZ()+zVel*dt) == true)
                {
                    Triangle3D* savedTri = CollisionChecker::getCollideTriangle();
                    Vector3f triColPosition(CollisionChecker::getCollidePosition());

                    float xTest =  10*cosf(Maths::toRadians(getRotY()));
                    float zTest = -10*sinf(Maths::toRadians(getRotY()));
                    if (CollisionChecker::checkCollision(getX()+xTest, getY(), getZ()+zTest, getX()+xTest, getY()-10.0f, getZ()+zTest) == true)
                    {
                        inAir = false;
                        triCol = savedTri;
                        //Vector3f* triColPosition = CollisionChecker::getCollidePosition();
                    
                        //yVel = 0;
                    
                        float colXNormal = triCol->normal.x;
                        float colYNormal = triCol->normal.y;
                        float colZNormal = triCol->normal.z;
                    
                        if (colYNormal >= 0.75f)
                        {
                            setX(triColPosition.x);
                            setY(triColPosition.y+colYNormal*0.5f);
                            setZ(triColPosition.z);
                        }
                        else
                        {
                            if (colYNormal >= 0.5f)
                            {
                                setX(triColPosition.x);
                                setY(triColPosition.y+colYNormal*0.5f);
                                setZ(triColPosition.z);
                                xVel+=colXNormal*0.05f*60*60*dt;
                                zVel+=colZNormal*0.05f*60*60*dt;
                            }
                            else
                            {
                                setX(triColPosition.x+colXNormal*0.5f);
                                setY(triColPosition.y+colYNormal*0.5f);
                                setZ(triColPosition.z+colZNormal*0.5f);
                                xVel = 0;
                                zVel = 0;
                                setRotY(getRotY()+180);
                            }
                        }
                    }
                    else //turn around if we are abound to drive off a cliff
                    {
                        setRotY(getRotY()+180);
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
                    setX(triColPosition->x);
                    setY(triColPosition->y+colYNormal*0.5f);
                    setZ(triColPosition->z);
                }
                else
                {
                    if (colYNormal >= 0.5f)
                    {
                        setX(triColPosition->x);
                        setY(triColPosition->y+colYNormal*0.5f);
                        setZ(triColPosition->z);
                        xVel+=colXNormal*0.05f*60*60*dt;
                        zVel+=colZNormal*0.05f*60*60*dt;
                    }
                    else
                    {
                        setX(triColPosition->x+colXNormal*0.5f);
                        setY(triColPosition->y+colYNormal*0.5f);
                        setZ(triColPosition->z+colZNormal*0.5f);
                        xVel = 0;
                        zVel = 0;
                        setRotY(getRotY()+180);
                    }
                }
            }
        }

        if (getY() < -50)
        {
            //die();
        }

        Maths::createTransformationMatrix(&transformationMatrix, &position, 0, rotY, 0, 0, 1);
    }
}

void RhinoSpike::die()
{
    AudioPlayer::play(3, &position);

    Main_deleteChunkedEntity(this);

    visible = false;

    float height = 10.0f;
    float spread = 20.0f;

    Vector3f vel(0, 0, 0);
    Vector3f toCamDiff = Global::gameCamera->eye - position;
    toCamDiff.setLength(20);
    
    for (int i = 7; i != 0; i--)
    {
        Vector3f pos(
            getX() + spread*(Maths::random() - 0.5f),
            getY() + spread*(Maths::random() - 0.5f) + height,
            getZ() + spread*(Maths::random() - 0.5f));
        pos = pos + toCamDiff; //so that these aren't behind the big explosion

        ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &vel, 0, 0.75f, 0, 3*Maths::random() + 6, 0, false, false, 0.5f, true);
    }
    
    Vector3f pos(
        getX(),
        getY() + height,
        getZ());
    
    ParticleMaster::createParticle(ParticleResources::textureExplosion2, &pos, &vel, 0, 0.916f, 0, 20, 0, false, false, 0.75f, true);
    
    Global::gameScore += 100;
}

bool RhinoSpike::playerIsInHurtbox()
{
    Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;
    float diffY = diff.y;
    diff.y = 0;
    float circleDistSquared = diff.lengthSquared();

    return (circleDistSquared <= RhinoSpike::hurtboxRadius*RhinoSpike::hurtboxRadius &&
        diffY >= RhinoSpike::hitboxHeight &&
        diffY <= RhinoSpike::hitboxHeight + RhinoSpike::hurtboxHeight);
}

bool RhinoSpike::playerIsInHitbox()
{
    Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;
    float diffY = diff.y;
    diff.y = 0;
    float circleDistSquared = diff.lengthSquared();

    return (circleDistSquared <= RhinoSpike::hitboxRadius*RhinoSpike::hitboxRadius &&
        diffY >= 0 &&
        diffY <= RhinoSpike::hitboxHeight);
}

void RhinoSpike::moveMe()
{
    //extern float dt;
    //xVel += (0.7f*dt*60*60*cosf(Maths::toRadians(-getRotY())));
    //zVel += (0.7f*dt*60*60*sinf(Maths::toRadians(-getRotY())));
    xVel = 150*cosf(Maths::toRadians(-getRotY()));
    zVel = 150*sinf(Maths::toRadians(-getRotY()));
}
    
void RhinoSpike::moveMeSlow()
{
    //extern float dt;
    //xVel += (0.08f*dt*60*60*cosf(Maths::toRadians(-getRotY())));
    //zVel += (0.08f*dt*60*60*sinf(Maths::toRadians(-getRotY())));
    xVel = 15*cosf(Maths::toRadians(-getRotY()));
    zVel = 15*sinf(Maths::toRadians(-getRotY()));
}
    
void RhinoSpike::applyFriction(float /*frictionToApply*/)
{
    //xVel = xVel/frictionToApply;
    //zVel = zVel/frictionToApply;
    xVel = 0;
    zVel = 0;
}

std::list<TexturedModel*>* RhinoSpike::getModels()
{
    return &RhinoSpike::models;
}

void RhinoSpike::loadStaticModels()
{
    if (RhinoSpike::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading RhinoSpike static models...\n");
    #endif

    loadModel(&RhinoSpike::models, "res/Models/Objects/RhinoSpike/", "RhinoSpike");
}

void RhinoSpike::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting RhinoSpike static models...\n");
    #endif

    Entity::deleteModels(&RhinoSpike::models);
}

const bool RhinoSpike::canHomingAttackOn()
{
    return true;
}

const Vector3f RhinoSpike::getHomingCenter()
{
    Vector3f homingCenter(&position);
    homingCenter.y += 13.2651f;
    return homingCenter;
}

const bool RhinoSpike::isEnemy()
{
    return true;
}
