#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "kiki.hpp"
#include "dummy.hpp"
#include "bomb.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> Kiki::models;
std::list<TexturedModel*> Kiki::modelsArm;

Kiki::Kiki()
{

}

Kiki::Kiki(
    float x, float y, float z,
    float dx, float dz,
    std::list<Entity*>* entityListToAdd)
{
    position.x = x;
    position.y = y;
    position.z = z;

    centerPos = position;
    centerPos.y += 3.80568f*scale;

    throwTimer = Maths::random()*timeUntilThrow + 0.5f;

    forward = Vector3f(dx, 0, dz);

    rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));
    
    updateTransformationMatrix();

    arm = new Dummy(&Kiki::modelsArm); INCR_NEW("Entity");
    arm->setVisible(true);
    arm->scale = scale;
    updateArm();
    arm->rotZ = -69.0f;
    arm->updateTransformationMatrix();
    entityListToAdd->push_back(arm);
}

extern float dt;

void Kiki::step()
{
    Vector3f diff = Global::gameMainPlayer->position - position;
    float distToPlayer = diff.lengthSquared();

    if (distToPlayer < Kiki::activationRange*Kiki::activationRange)
    {
        float timerBefore = throwTimer;
        throwTimer -= dt;

        if (distToPlayer < (6.28505f*scale)*(6.28505f*scale) &&
            diff.y >= 0.0f &&
            diff.y <= 8.27256f*scale)
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
        diff.y = 0;
        diff.normalize();
        forward.normalize();

        float angToPlayer = Maths::angleBetweenVectors(&forward, &diff);
        if (angToPlayer > 0.3f)
        {
            forward = Maths::interpolateVector(&forward, &diff, Kiki::lookSpeed*dt);
            rotY = Maths::toDegrees(atan2f(-forward.z, forward.x));

            updateTransformationMatrix();
        }

        updateArm();

        if (throwTimer >= 0.5f)
        {
            if (arm->rotZ != -69.0f)
            {
                arm->rotZ = -69.0f;
                arm->updateTransformationMatrix();
            }
        }
        else if (throwTimer >= 0.0f)
        {
            arm->rotZ = 55.0f - (2*throwTimer)*124.0f;
            arm->updateTransformationMatrix();
        }
        else if (throwTimer >= -0.25f)
        {
            arm->rotZ = 55.0f + (4*throwTimer)*124.0f;
            arm->updateTransformationMatrix();

            if (timerBefore >= 0.0f)
            {
                Vector3f diff2 = Global::gameMainPlayer->position - position;
                diff2.y = 0;

                AudioPlayer::play(72, &position);
                Bomb* bomb = new Bomb(position.x, position.y + 6, position.z, 1.4f*diff2.x, 100, 1.4f*diff2.z); INCR_NEW("Entity");
                Global::addEntity(bomb);
            }
        }
        else
        {
            throwTimer = Maths::random()*timeUntilThrow + 0.5f;
        }
    }
}

void Kiki::updateArm()
{
    //calc position of shoulder
    Vector3f yAxis(0, 1, 0);
    Vector3f right = forward.cross(&yAxis);

    float upOffset = 3.72936f*scale;
    float rightOffset = 0.800129f*scale;
    float forwardOffset = 0.364575f*scale;

    Vector3f armPos = position + yAxis.scaleCopy(upOffset) + right.scaleCopy(rightOffset) + forward.scaleCopy(forwardOffset);
    arm->position = armPos;

    arm->rotY = rotY;
}

void Kiki::die()
{
    AudioPlayer::play(3, &position);
    
    Global::deleteChunkedEntity(this);
    Global::deleteChunkedEntity(arm);

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

        ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &vel, 0, 0.75f, 3*Maths::random() + 6, 0, false, false, 0.5f, true);
    }
    
    Vector3f pos(
        getX(),
        getY() + height,
        getZ());
    
    ParticleMaster::createParticle(ParticleResources::textureExplosion2, &pos, &vel, 0, 0.916f, 20, 0, false, false, 0.75f, true);
    
    Global::gameScore += 100;
}

std::list<TexturedModel*>* Kiki::getModels()
{
    return &Kiki::models;
}

void Kiki::loadStaticModels()
{
    if (Kiki::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Kiki static models...\n");
    #endif

    ModelLoader::loadModel(&Kiki::models,    "res/Models/Objects/Kiki/", "Kiki");
    ModelLoader::loadModel(&Kiki::modelsArm, "res/Models/Objects/Kiki/", "Arm");
}

void Kiki::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Kiki static models...\n");
    #endif

    Entity::deleteModels(&Kiki::models);
    Entity::deleteModels(&Kiki::modelsArm);
}

bool Kiki::canHomingAttackOn() const
{
    return true;
}

Vector3f Kiki::getHomingCenter() const
{
    return centerPos;
}

bool Kiki::isEnemy() const
{
    return true;
}
