#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "e1000.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "controllableplayer.h"
#include "../toolbox/maths.h"
#include "camera.h"
#include "dummy.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../particles/particlemaster.h"
#include "../collision/collisionchecker.h"
#include "bullet.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> E1000::modelsBody;
std::list<TexturedModel*> E1000::modelsGun;

E1000::E1000()
{

}

E1000::E1000(float x, float y, float z, float dx, float dz, std::list<Entity*>* entityListToAdd)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 1.0f;
    visible = true;

    if (CollisionChecker::checkCollision(
        position.x, position.y+10, position.z, 
        position.x, position.y-1000, position.z))
    {
        position = CollisionChecker::getCollidePosition();
    }

    centerPos = position;
    centerPos.y += 14.5f*scale;

    forward.set(dx, 0, dz);
    forward.normalize();
    rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));

    updateTransformationMatrix();

    gun1 = new Dummy(&E1000::modelsGun); INCR_NEW("Entity");
    gun1->setVisible(true);
    gun1->setPosition(&position);
    gun1->scale = scale;
    updateGun1();
    gun1->rotY = rotY;
    gun1->rotZ = 0;
    gun1->updateTransformationMatrix();
    entityListToAdd->push_back(gun1);

    gun2 = new Dummy(&E1000::modelsGun); INCR_NEW("Entity");
    gun2->setVisible(true);
    gun2->setPosition(&position);
    gun2->scale = scale;
    updateGun2();
    gun2->rotY = rotY;
    gun2->rotZ = 0;
    gun2->updateTransformationMatrix();
    entityListToAdd->push_back(gun2);
}

void E1000::updateGun1()
{
    //calc position of shoulder
    Vector3f yAxis(0, 1, 0);
    Vector3f right = forward.cross(&yAxis);

    float upOffset = 12.6444f*scale;
    float rightOffset = 6.39557f*scale;
    float forwardOffset = 1.77027f*scale;

    Vector3f gunPos = position + yAxis.scaleCopy(upOffset) + right.scaleCopy(rightOffset) + forward.scaleCopy(forwardOffset);
    gun1->position = gunPos;

    float hitInThisSec = 0.8f;

    //point the gun inbetween sonics current position and where he will be shortly
    Vector3f target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec*0.25f);
    Vector3f diff = target - gunPos;
    gun1->rotY = Maths::toDegrees(atan2f(-diff.z, diff.x));
    gun1->rotZ = Maths::toDegrees(atan2f(diff.y, sqrtf(diff.x*diff.x + diff.z*diff.z)));
    gun1->updateTransformationMatrix();
    gun1Direction = diff; //gun visual direction
    gun1Direction.normalize();
    gun1TipPos = gunPos + gun1Direction.scaleCopy(7.14889f*scale);

    //figure out where sonic is going to be in 0.5 seconds, aim the bullet there
    target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec);

    diff = target - gun1TipPos;

    bullet1Direction = diff;
    bullet1Direction.normalize();
    bullet1TipPos = gun1TipPos;

    Vector3f distToCover = target - bullet1TipPos;
    bullet1Speed = distToCover.length()/hitInThisSec;

    bullet1Speed = fmaxf(bullet1Speed, 50.0f);
}

void E1000::updateGun2()
{
    //calc position of shoulder
    Vector3f yAxis(0, 1, 0);
    Vector3f right = forward.cross(&yAxis);

    float upOffset = 12.6444f*scale;
    float rightOffset = -6.39557f*scale;
    float forwardOffset = 1.77027f*scale;

    Vector3f gunPos = position + yAxis.scaleCopy(upOffset) + right.scaleCopy(rightOffset) + forward.scaleCopy(forwardOffset);
    gun2->position = gunPos;

    float hitInThisSec = 0.75f;

    //point the gun inbetween sonics current position and where he will be shortly
    Vector3f target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec*0.25f);
    Vector3f diff = target - gunPos;
    gun2->rotY = Maths::toDegrees(atan2f(-diff.z, diff.x));
    gun2->rotZ = Maths::toDegrees(atan2f(diff.y, sqrtf(diff.x*diff.x + diff.z*diff.z)));
    gun2->updateTransformationMatrix();
    gun2Direction = diff; //gun visual direction
    gun2Direction.normalize();
    gun2TipPos = gunPos + gun2Direction.scaleCopy(7.14889f*scale);

    //figure out where sonic is going to be in 0.5 seconds, aim the bullet there
    target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec);

    diff = target - gun2TipPos;

    bullet2Direction = diff;
    bullet2Direction.normalize();
    bullet2TipPos = gun2TipPos;

    Vector3f distToCover = target - bullet2TipPos;
    bullet2Speed = distToCover.length()/hitInThisSec;

    bullet2Speed = fmaxf(bullet2Speed, 50.0f);
}

void E1000::step()
{
    extern float dt;

    if (fabsf(position.x - Global::gameMainPlayer->position.x) < E1000::activationRange &&
        fabsf(position.y - Global::gameMainPlayer->position.y) < E1000::activationRange &&
        fabsf(position.z - Global::gameMainPlayer->position.z) < E1000::activationRange)
    {
        shootTimer+=dt;

        Vector3f toPlayerDiff = Global::gameMainPlayer->position - position;
        float toPlayerDiffY = toPlayerDiff.y;
        toPlayerDiff.y = 0;

        if ((toPlayerDiff.lengthSquared()) < (hitRadius*scale)*(hitRadius*scale) &&
            toPlayerDiffY >= 0 && toPlayerDiffY <= hitHeight*scale)
        {
            if (Global::gameMainPlayer->canDealDamage())
            {
                die();
                Global::gameMainPlayer->rebound(&centerPos);
                Global::gameMainPlayer->increaseCombo();
                return;
            }
            else
            {
                Global::gameMainPlayer->takeDamage(&position);
            }
        }

        //rotate toward player
        toPlayerDiff.y = 0;
        toPlayerDiff.normalize();
        forward.normalize();

        float angToPlayer = Maths::angleBetweenVectors(&forward, &toPlayerDiff);
        if (angToPlayer > 0.3f)
        {
            forward = Maths::interpolateVector(&forward, &toPlayerDiff, E1000::lookSpeed*dt);
            rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));
        }

        updateTransformationMatrix();
        updateGun1();
        updateGun2();

        if (shootTimer > timeUntilShoot)
        {
            shootTimer = Maths::random()*0.2f;

            Vector3f bullet1Vel = bullet1Direction.scaleCopy(bullet1Speed);
            Bullet* bullet1 = new Bullet(&bullet1TipPos, &bullet1Vel, timeUntilShoot*3.0f); INCR_NEW("Entity");
            Global::addEntity(bullet1);

            Vector3f bullet2Vel = bullet2Direction.scaleCopy(bullet2Speed);
            Bullet* bullet2 = new Bullet(&bullet2TipPos, &bullet2Vel, timeUntilShoot*3.0f); INCR_NEW("Entity");
            Global::addEntity(bullet2);
        }
    }
    else
    {
        shootTimer = 0.0f;
    }
}

void E1000::die()
{
    AudioPlayer::play(3, &position);
    
    Global::deleteChunkedEntity(this);
    Global::deleteChunkedEntity(gun1);
    Global::deleteChunkedEntity(gun2);

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

std::list<TexturedModel*>* E1000::getModels()
{
    return &E1000::modelsBody;
}

void E1000::loadStaticModels()
{
    if (E1000::modelsBody.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading E1000 static models...\n");
    #endif

    ObjLoader::loadModel(&E1000::modelsBody,   "res/Models/Objects/E1000/", "E1000");
    ObjLoader::loadModel(&E1000::modelsGun,    "res/Models/Objects/E1000/", "Gun");
}

void E1000::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting E1000 static models...\n");
    #endif

    Entity::deleteModels(&E1000::modelsBody);
    Entity::deleteModels(&E1000::modelsGun);
}

bool E1000::canHomingAttackOn() const
{
    return true;
}

Vector3f E1000::getHomingCenter() const
{
    return centerPos;
}

bool E1000::isEnemy() const
{
    return true;
}
