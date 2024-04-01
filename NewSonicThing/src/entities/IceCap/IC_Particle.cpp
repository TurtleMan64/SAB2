#include "icparticle.hpp"
#include "../../particles/particletexture.hpp"
#include "../camera.hpp"
#include "../../toolbox/vector.hpp"
#include "../../particles/particlemaster.hpp"
#include "../../particles/particleresources.hpp"
#include "../../engineTester/main.hpp"
#include "../../toolbox/maths.hpp"

extern float dt;

int IC_Particle::currentPopulation = 0;
Vector3f IC_Particle::centerSpawnPos;

IC_Particle::IC_Particle()
{
    texture = ParticleResources::textureSnowball;
    positionRef = &position;
    gravityEffect = 0;
    lifeLength = 3.0f;
    scaleChange = -0.01f;
    onlyRendersOnce = false;
    opacity = 1.0f;
    respawn();
    elapsedTime = Maths::random()*4.0f;
    updateTextureCoordInfo();
    IC_Particle::currentPopulation++;
    ParticleMaster::addParticleIC(this);
}

IC_Particle::~IC_Particle()
{
    IC_Particle::currentPopulation--;
}

void IC_Particle::respawn()
{
    elapsedTime = 0;
    position.x = centerSpawnPos.x + snowRadius*2*Maths::random();
    position.y = centerSpawnPos.y + snowRadius*(1 + Maths::random());
    position.z = centerSpawnPos.z + snowRadius*2*Maths::random();
    velocity.x = (Maths::random())*30 + 60;
    velocity.y = (Maths::random())*30 - 270;
    velocity.z = (Maths::random())*30 + 60;
    scale = Maths::random()*2.0f + 0.5f;
}

bool IC_Particle::update()
{
    elapsedTime += dt;

    if (elapsedTime >= 4.0f)
    {
        respawn();
    }

    scale = fmaxf(0, scale - 0.1f*dt);

    position.x += velocity.x*dt;
    position.y += velocity.y*dt;
    position.z += velocity.z*dt;

    updateTextureCoordInfo();

    return Global::levelId == LVL_ICE_CAP;
}
