#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "beetle.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "dummy.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::vector<TexturedModel*> Beetle::modelsBody;
std::vector<TexturedModel*> Beetle::modelsBlades;

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

    lookDir.set(1, 0, 0);
    lookDir = Maths::rotatePoint(&lookDir, &Y_AXIS, Maths::random()*Maths::PI*2);
    rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));

    updateTransformationMatrix();

    bobTimer = Maths::nextUniform()*2;

    blades = new Dummy(&Beetle::modelsBlades); INCR_NEW("Entity");
    blades->visible = (true);
    blades->setPosition(&position);
    updateBlades(&position);
    entityListToAdd->push_back(blades);

    //baseColor.set(Maths::nextUniform()*5, Maths::nextUniform()*5, Maths::nextUniform()*5);
    //blades->baseColor.set(&baseColor);
}

void Beetle::updateBlades(Vector3f* pos)
{
    //calculate new blades position
    Vector3f spinOff(-5.87751f, 0, 0);
    spinOff = Maths::rotatePoint(&spinOff, &Y_AXIS, Maths::toRadians(rotY));
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

    if (std::abs(position.x - Global::gameMainPlayer->position.x) < Beetle::activationRange &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < Beetle::activationRange &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < Beetle::activationRange)
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

std::vector<TexturedModel*>* Beetle::getModels()
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
    printf("Loading Beetle static models...\n");
    #endif

    ModelLoader::loadModel(&Beetle::modelsBody,   "res/Models/Objects/Beetle/", "Beetle");
    ModelLoader::loadModel(&Beetle::modelsBlades, "res/Models/Objects/Beetle/", "Blades");
}

void Beetle::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Beetle static models...\n");
    #endif

    Entity::deleteModels(&Beetle::modelsBody);
    Entity::deleteModels(&Beetle::modelsBlades);
}

bool Beetle::canHomingAttackOn() const
{
    return true;
}

Vector3f Beetle::getHomingCenter() const
{
    return position;
}

bool Beetle::isEnemy() const
{
    return true;
}
