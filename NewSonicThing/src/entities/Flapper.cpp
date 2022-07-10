#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "flapper.hpp"
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
#include "../audio/audioplayer.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> Flapper::modelsRedBody;
std::list<TexturedModel*> Flapper::modelsRedWingRight;
std::list<TexturedModel*> Flapper::modelsRedWingLeft;
std::list<TexturedModel*> Flapper::modelsGreenBody;
std::list<TexturedModel*> Flapper::modelsGreenWingRight;
std::list<TexturedModel*> Flapper::modelsGreenWingLeft;
std::list<TexturedModel*> Flapper::modelsGun;

Flapper::Flapper()
{

}

Flapper::Flapper(float x, float y, float z, int t, float dx, float dz, std::list<Entity*>* entityListToAdd)
{
    position.x = x;
    position.y = y;
    position.z = z;
    visible = true;

    type = t;

    forward.set(dx, 0, dz);
    forward.normalize();
    rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));

    updateTransformationMatrix();

    if (type == 1)
    {
        gun = new Dummy(&Flapper::modelsGun); INCR_NEW("Entity");
        updateGun();
        gun->rotY = rotY;
        gun->rotZ = 0;
        gun->updateTransformationMatrix();
        entityListToAdd->push_back(gun);
    }

    if (type == 0)
    {
        wingLeft  = new Dummy(&Flapper::modelsRedWingLeft);  INCR_NEW("Entity");
        wingRight = new Dummy(&Flapper::modelsRedWingRight); INCR_NEW("Entity");
    }
    else
    {
        wingLeft  = new Dummy(&Flapper::modelsGreenWingLeft);  INCR_NEW("Entity");
        wingRight = new Dummy(&Flapper::modelsGreenWingRight); INCR_NEW("Entity");
    }
    updateWings();

    entityListToAdd->push_back(wingLeft);
    entityListToAdd->push_back(wingRight);

    //baseColor.set(Maths::nextUniform()*5, Maths::nextUniform()*5, Maths::nextUniform()*5);
    //gun->baseColor.set(&baseColor);
}

void Flapper::updateGun()
{
    //calc position of the gun
    gun->position = position;
    gun->position.y = position.y - 4.6f;

    float hitInThisSec = 0.5f;

    //point the gun inbetween sonics current position and where he will be shortly
    Vector3f target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec*0.25f);
    Vector3f diff = target - gun->position;
    gun->rotY = Maths::toDegrees(atan2f(-diff.z, diff.x));
    gun->rotZ = Maths::toDegrees(atan2f(diff.y, sqrtf(diff.x*diff.x + diff.z*diff.z)));
    gun->updateTransformationMatrix();
    gunDirection = diff; //gun visual direction
    gunDirection.normalize();
    gunTipPos = gun->position + gunDirection.scaleCopy(10.13f);

    //figure out where sonic is going to be in 0.5 seconds, aim the bullet there
    target = Global::gameMainPlayer->getCenterPosition() + Global::gameMainPlayer->vel.scaleCopy(hitInThisSec);

    diff = target - gunTipPos;

    bulletDirection = diff;
    bulletDirection.normalize();
    bulletTipPos = gunTipPos;

    Vector3f distToCover = target - bulletTipPos;
    bulletSpeed = distToCover.length()/hitInThisSec;

    bulletSpeed = fmaxf(bulletSpeed, 50.0f);
}

void Flapper::updateWings()
{
    Vector3f right = forward.cross(&Y_AXIS);
    float upOffset = -0.723151f;
    float rightOffset = 7.80119f;
    Vector3f wingPos1 = position + Y_AXIS.scaleCopy(upOffset) + right.scaleCopy(rightOffset);
    Vector3f wingPos2 = position + Y_AXIS.scaleCopy(upOffset) + right.scaleCopy(-rightOffset);
    wingLeft->position = wingPos1;
    wingRight->position = wingPos2;

    wingLeft->transformationMatrix.setIdentity();
    wingLeft->transformationMatrix.translate(&wingLeft->position);
    wingLeft->transformationMatrix.rotate(Maths::toRadians(rotY), &Y_AXIS);
    wingLeft->transformationMatrix.rotate(sinf(Global::gameClock*18)/8, &X_AXIS);
    
    wingRight->transformationMatrix.setIdentity();
    wingRight->transformationMatrix.translate(&wingRight->position);
    wingRight->transformationMatrix.rotate(Maths::toRadians(rotY), &Y_AXIS);
    wingRight->transformationMatrix.rotate(-sinf(Global::gameClock*18)/8, &X_AXIS);
}

void Flapper::step()
{
    extern float dt;

    if (std::abs(position.x - Global::gameMainPlayer->position.x) < Flapper::activationRange &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < Flapper::activationRange &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < Flapper::activationRange)
    {
        shootTimer+=dt;

        Vector3f toPlayerDiff = Global::gameMainPlayer->position - position;

        if ((toPlayerDiff.lengthSquared()) < (hitRadius)*(hitRadius))
        {
            if (Global::gameMainPlayer->canDealDamage())
            {
                die();
                Global::gameMainPlayer->rebound(&position);
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
            forward = Maths::interpolateVector(&forward, &toPlayerDiff, Flapper::lookSpeed*dt);
            rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));
        }

        updateTransformationMatrix();

        if (type == 1)
        {
            updateGun();

            if (shootTimer > timeUntilShoot)
            {
                AudioPlayer::play(75, &position);

                shootTimer = Maths::random()*0.2f;

                Vector3f bulletVel = bulletDirection.scaleCopy(bulletSpeed);
                Bullet* bullet = new Bullet(&bulletTipPos, &bulletVel, timeUntilShoot*3.0f); INCR_NEW("Entity");
                Global::addEntity(bullet);
            }
        }

        updateWings();

        //TODO add particles for engine exhaust
    }
    else
    {
        shootTimer = 0.0f;
    }
}

void Flapper::die()
{
    AudioPlayer::play(3, &position);
    
    Global::deleteChunkedEntity(this);
    Global::deleteChunkedEntity(wingLeft);
    Global::deleteChunkedEntity(wingRight);

    if (type == 1)
    {
        Global::deleteChunkedEntity(gun);
    }

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

std::list<TexturedModel*>* Flapper::getModels()
{
    if (type == 0)
    {
        return &Flapper::modelsRedBody;
    }
    
    return &Flapper::modelsGreenBody;
}

void Flapper::loadStaticModels()
{
    if (Flapper::modelsRedBody.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Flapper static models...\n");
    #endif

    ModelLoader::loadModel(&Flapper::modelsRedBody,        "res/Models/Objects/Flapper/Red/", "Flapper");
    ModelLoader::loadModel(&Flapper::modelsRedWingRight,   "res/Models/Objects/Flapper/Red/", "WingLeft");
    ModelLoader::loadModel(&Flapper::modelsRedWingLeft,    "res/Models/Objects/Flapper/Red/", "WingRight");
    ModelLoader::loadModel(&Flapper::modelsGreenBody,      "res/Models/Objects/Flapper/Green/", "Flapper");
    ModelLoader::loadModel(&Flapper::modelsGreenWingRight, "res/Models/Objects/Flapper/Green/", "WingLeft");
    ModelLoader::loadModel(&Flapper::modelsGreenWingLeft,  "res/Models/Objects/Flapper/Green/", "WingRight");
    ModelLoader::loadModel(&Flapper::modelsGun,            "res/Models/Objects/Flapper/", "Cannon");
}

void Flapper::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Flapper static models...\n");
    #endif

    Entity::deleteModels(&Flapper::modelsRedBody);
    Entity::deleteModels(&Flapper::modelsRedWingRight);
    Entity::deleteModels(&Flapper::modelsRedWingLeft);
    Entity::deleteModels(&Flapper::modelsGreenBody);
    Entity::deleteModels(&Flapper::modelsGreenWingRight);
    Entity::deleteModels(&Flapper::modelsGreenWingLeft);
    Entity::deleteModels(&Flapper::modelsGun);
}

bool Flapper::canHomingAttackOn() const
{
    return true;
}

Vector3f Flapper::getHomingCenter() const
{
    return position;
}

bool Flapper::isEnemy() const
{
    return true;
}
