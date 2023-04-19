#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "hunter.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "camera.hpp"
#include "dummy.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"
#include "../collision/collisionchecker.hpp"
#include "bullet.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> Hunter::modelsBody;
std::vector<TexturedModel*> Hunter::modelsGun;

Hunter::Hunter()
{

}

Hunter::Hunter(float x, float y, float z, float dx, float dz, std::list<Entity*>* entityListToAdd)
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
    centerPos.y += 21.3381f*scale;

    forward.set(dx, 0, dz);
    forward.normalize();
    rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));

    updateTransformationMatrix();

    gun = new Dummy(&Hunter::modelsGun); INCR_NEW("Entity");
    gun->visible = (true);
    gun->setPosition(&position);
    gun->scale = scale;
    updateGun();
    gun->rotY = rotY;
    gun->rotZ = 0;
    gun->updateTransformationMatrix();
    entityListToAdd->push_back(gun);

    //baseColor.set(Maths::nextUniform()*5, Maths::nextUniform()*5, Maths::nextUniform()*5);
    //gun->baseColor.set(&baseColor);
}

void Hunter::updateGun()
{
    //calc position of shoulder
    Vector3f right = forward.cross(&Y_AXIS);
    float upOffset = 23.31378f*scale;
    float rightOffset = 8.265114f*scale;
    Vector3f gunPos = position + Y_AXIS.scaleCopy(upOffset) + right.scaleCopy(rightOffset);
    gun->position = gunPos;

    float hitInThisSec = 0.5f;

    //point the gun inbetween sonics current position and where he will be shortly
    Vector3f target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec*0.25f);
    Vector3f diff = target - gunPos;
    gun->rotY = Maths::toDegrees(atan2f(-diff.z, diff.x));
    gun->rotZ = Maths::toDegrees(atan2f(diff.y, sqrtf(diff.x*diff.x + diff.z*diff.z)));
    gun->updateTransformationMatrix();
    gunDirection = diff; //gun visual direction
    gunDirection.normalize();
    gunTipPos = gunPos + gunDirection.scaleCopy(22.5189f*scale);

    //figure out where sonic is going to be in 0.5 seconds, aim the bullet there
    target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec);

    diff = target - gunTipPos;

    bulletDirection = diff;
    bulletDirection.normalize();
    bulletTipPos = gunTipPos;//gunPos + bulletDirection.scaleCopy(22.5189f*scale);

    Vector3f distToCover = target - bulletTipPos;
    bulletSpeed = distToCover.length()/hitInThisSec;

    bulletSpeed = fmaxf(bulletSpeed, 50.0f);
}

void Hunter::step()
{
    extern float dt;

    if (std::abs(position.x - Global::gameMainPlayer->position.x) < Hunter::activationRange &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < Hunter::activationRange &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < Hunter::activationRange)
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
                Global::gameMainPlayer->takeDamage(&centerPos);
            }
        }

        //rotate toward player
        toPlayerDiff.y = 0;
        toPlayerDiff.normalize();
        forward.normalize();

        float angToPlayer = Maths::angleBetweenVectors(&forward, &toPlayerDiff);
        if (angToPlayer > 0.3f)
        {
            forward = Maths::interpolateVector(&forward, &toPlayerDiff, Hunter::lookSpeed*dt);
            rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));
        }

        updateTransformationMatrix();
        updateGun();

        if (shootTimer > timeUntilShoot)
        {
            shootTimer = Maths::random()*0.2f;

            Vector3f bulletVel = bulletDirection.scaleCopy(bulletSpeed);
            Bullet* bullet = new Bullet(&bulletTipPos, &bulletVel, timeUntilShoot*3.0f); INCR_NEW("Entity");
            Global::addEntity(bullet);
        }
    }
    else
    {
        shootTimer = 0.0f;
    }
}

void Hunter::die()
{
    AudioPlayer::play(3, &position);
    
    Global::deleteChunkedEntity(this);
    Global::deleteChunkedEntity(gun);

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

std::vector<TexturedModel*>* Hunter::getModels()
{
    return &Hunter::modelsBody;
}

void Hunter::loadStaticModels()
{
    if (Hunter::modelsBody.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Hunter static models...\n");
    #endif

    ModelLoader::loadModel(&Hunter::modelsBody,   "res/Models/Objects/Hunter/", "Hunter");
    ModelLoader::loadModel(&Hunter::modelsGun,    "res/Models/Objects/Hunter/", "Gun");
}

void Hunter::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Hunter static models...\n");
    #endif

    Entity::deleteModels(&Hunter::modelsBody);
    Entity::deleteModels(&Hunter::modelsGun);
}

bool Hunter::canHomingAttackOn() const
{
    return true;
}

Vector3f Hunter::getHomingCenter() const
{
    return centerPos;
}

bool Hunter::isEnemy() const
{
    return true;
}
