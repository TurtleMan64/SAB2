#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "spinner.hpp"
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

std::vector<TexturedModel*> Spinner::modelsBody;
std::vector<TexturedModel*> Spinner::modelsBlades;

Spinner::Spinner()
{

}

Spinner::Spinner(float x, float y, float z, std::list<Entity*>* entityListToAdd)
{
    position.x = x;
    position.y = y;
    position.z = z;

    lookDir.set(1, 0, 0);
    lookDir = Maths::rotatePoint(&lookDir, &Y_AXIS, Maths::random()*Maths::PI*2);
    rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));

    updateTransformationMatrix();

    bobTimer = Maths::nextUniform()*2;

    blades = new Dummy(&Spinner::modelsBlades); INCR_NEW("Entity");
    blades->visible = (true);
    blades->setPosition(&position);
    updateBlades(&position);
    entityListToAdd->push_back(blades);
}

void Spinner::updateBlades(Vector3f* pos)
{
    blades->position = pos;
    blades->updateTransformationMatrix();
}

void Spinner::step()
{
    extern float dt;
    bobTimer += dt;
    
    Vector3f posBob(&position);
    posBob.y += Spinner::bobHeight*sinf((bobTimer*Maths::PI)*Spinner::bobPeriodScale);
    blades->rotY += 1200*dt;

    if (std::abs(position.x - Global::gameMainPlayer->position.x) < Spinner::activationRange &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < Spinner::activationRange &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < Spinner::activationRange)
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
        lookDir = Maths::interpolateVector(&lookDir, &toPlayerDiff, Spinner::lookSpeed*dt);
        rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));
    }

    Maths::createTransformationMatrix(&transformationMatrix, &posBob, 0, rotY, 0, 0, 1);
    updateBlades(&posBob);
}

void Spinner::die()
{
    AudioPlayer::play(3, &position);

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

std::vector<TexturedModel*>* Spinner::getModels()
{
    return &Spinner::modelsBody;
}

void Spinner::loadStaticModels()
{
    if (Spinner::modelsBody.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Spinner static models...\n");
    #endif

    ModelLoader::loadModel(&Spinner::modelsBody,   "res/Models/Objects/Spinner/", "Body");
    ModelLoader::loadModel(&Spinner::modelsBlades, "res/Models/Objects/Spinner/", "Blades");
}

void Spinner::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Spinner static models...\n");
    #endif

    Entity::deleteModels(&Spinner::modelsBody);
    Entity::deleteModels(&Spinner::modelsBlades);
}

bool Spinner::canHomingAttackOn() const
{
    return true;
}

Vector3f Spinner::getHomingCenter() const
{
    return position;
}

bool Spinner::isEnemy() const
{
    return true;
}
