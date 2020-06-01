#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "beetle.h"
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
#include "../particles/particlemaster.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Beetle::modelsBody;
std::list<TexturedModel*> Beetle::modelsBlades;

Beetle::Beetle()
{

}

Beetle::Beetle(float x, float y, float z, std::list<Entity*>* entityListToAdd)
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

    bobTimer = Maths::nextUniform()*2;

    blades = new Dummy(&Beetle::modelsBlades); INCR_NEW("Entity");
    blades->setVisible(true);
    blades->setPosition(&position);
    updateBlades(&position);
    entityListToAdd->push_back(blades);

    //baseColour.set(Maths::nextUniform()*5, Maths::nextUniform()*5, Maths::nextUniform()*5);
    //blades->baseColour.set(&baseColour);
}

void Beetle::updateBlades(Vector3f* pos)
{
    //calculate new blades position
    Vector3f spinOff(-5.87751f, 0, 0);
    Vector3f yAxis(0, 1, 0);
    spinOff = Maths::rotatePoint(&spinOff, &yAxis, Maths::toRadians(rotY));
    blades->position = spinOff + pos;
    blades->updateTransformationMatrix();
}

void Beetle::step()
{
    extern float dt;
    bobTimer += dt;
    
    Vector3f posBob(&position);
    posBob.y += Beetle::bobHeight*sinf((bobTimer*Maths::PI)*Beetle::bobPeriodScale);
    blades->rotY += 360*dt;

    if (fabsf(position.x - Global::gameMainPlayer->position.x) < Beetle::activationRange &&
        fabsf(position.y - Global::gameMainPlayer->position.y) < Beetle::activationRange &&
        fabsf(position.z - Global::gameMainPlayer->position.z) < Beetle::activationRange)
    {
        Vector3f toPlayerDiff = Global::gameMainPlayer->position - position;

        if ((toPlayerDiff.lengthSquared()) < hitRadius*hitRadius)
        {
            if (Global::gameMainPlayer->isVulnerable())
            {
                Global::gameMainPlayer->takeDamage(&position);
            }
            else if (Global::gameMainPlayer->canDealDamage())
            {
                die();
                Global::gameMainPlayer->rebound(&position);
                Global::gameMainPlayer->increaseCombo();
            }
        }

        //rotate toward player
        toPlayerDiff.y = 0;
        toPlayerDiff.normalize();
        lookDir = Maths::interpolateVector(&lookDir, &toPlayerDiff, Beetle::lookSpeed*dt);
        rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));
    }

    Maths::createTransformationMatrix(&transformationMatrix, &posBob, 0, rotY, 0, 0, 1);
    updateBlades(&posBob);
}

void Beetle::die()
{
    AudioPlayer::play(3, &position);

    //very strange bug that i cant figure out. sometimes when you homing attack a beetle and hit it, 
    // there will be a visible entity that returns a nullptr for their models during rendering.
    // im assuming that the entity is either the beetle or the blades. setting boths visibility to
    // false should fix the problem, but the source of the bug remains a mystery...
    // edit: this is because of the blades being in a different chunk than the one they were spawned in on,
    // because the blades change position when the beetle spins. i have edited the deleteChunkedEntity
    // function to deal with these special cases.
    //visible = false;
    //blades->visible = false;

    Global::deleteChunkedEntity(this);
    Global::deleteChunkedEntity(blades);

    const float height = 10.0f;
    const float spread = 20.0f;

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

std::list<TexturedModel*>* Beetle::getModels()
{
    return &Beetle::modelsBody;
}

void Beetle::loadStaticModels()
{
    if (Beetle::modelsBody.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading Beetle static models...\n");
    #endif

    ObjLoader::loadModel(&Beetle::modelsBody,   "res/Models/Objects/Beetle/", "Beetle");
    ObjLoader::loadModel(&Beetle::modelsBlades, "res/Models/Objects/Beetle/", "Blades");
}

void Beetle::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting Beetle static models...\n");
    #endif

    Entity::deleteModels(&Beetle::modelsBody);
    Entity::deleteModels(&Beetle::modelsBlades);
}

const bool Beetle::canHomingAttackOn()
{
    return true;
}

const Vector3f Beetle::getHomingCenter()
{
    return position;
}

const bool Beetle::isEnemy()
{
    return true;
}
