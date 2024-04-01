#include "caparticle.hpp"
#include "../../particles/particletexture.hpp"
#include "../camera.hpp"
#include "../../toolbox/vector.hpp"
#include "../../particles/particlemaster.hpp"
#include "../../particles/particleresources.hpp"
#include "../../engineTester/main.hpp"
#include "../../toolbox/maths.hpp"

extern float dt;

int CA_Particle::currentPopulation = 0;
Vector3f CA_Particle::centerSpawnPos;

CA_Particle::CA_Particle()
{
    texture = ParticleResources::textureRain;
    positionRef = &position;
    gravityEffect = 0;
    lifeLength = 1.0f;
    scaleChange = 0.0f;
    onlyRendersOnce = false;
    opacity = 1.0f;
    respawn();
    elapsedTime = Maths::random()*4.0f;
    updateTextureCoordInfo();
    CA_Particle::currentPopulation++;
    ParticleMaster::addParticleCA(this);
}

CA_Particle::~CA_Particle()
{
    CA_Particle::currentPopulation--;
}

void CA_Particle::respawn()
{
    elapsedTime = Maths::random()*10;
    position.x = centerSpawnPos.x + rainRadius*2*Maths::random();
    position.y = centerSpawnPos.y + rainRadius*(1 + Maths::random());
    position.z = centerSpawnPos.z + rainRadius*2*Maths::random();
    velocity.x = (Maths::random())*2 - 1;
    velocity.y = (Maths::random())*0 - 270;
    velocity.z = (Maths::random())*2 - 1;
    scale = Maths::random()*0.0f + 3.5f;
}

bool CA_Particle::update()
{
    elapsedTime += dt;

    if (elapsedTime >= 1.0f)
    {
        respawn();
    }

    //scale = fmaxf(0, scale - 0.1f*dt);

    position.x += velocity.x*dt;
    position.y += velocity.y*dt;
    position.z += velocity.z*dt;

    updateTextureCoordInfo();

    return Global::levelId == LVL_CASTLE_TOWN;
}
