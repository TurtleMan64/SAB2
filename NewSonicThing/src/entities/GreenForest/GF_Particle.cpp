#include "gfparticle.hpp"
#include "../../particles/particletexture.hpp"
#include "../camera.hpp"
#include "../../toolbox/vector.hpp"
#include "../../particles/particlemaster.hpp"
#include "../../particles/particleresources.hpp"
#include "../../engineTester/main.hpp"
#include "../../toolbox/maths.hpp"

extern float dt;

int GF_Particle::currentPopulation = 0;
Vector3f GF_Particle::centerSpawnPos;

GF_Particle::GF_Particle()
{
    texture = ParticleResources::texturePollen;
    positionRef = &position;
    gravityEffect = 0;
    lifeLength = 4.0f;
    scaleChange = -0.1f;
    onlyRendersOnce = false;
    opacity = 1.0f;
    respawn();
    elapsedTime = Maths::random()*4.0f;
    updateTextureCoordInfo();
    GF_Particle::currentPopulation++;
    ParticleMaster::addParticleGF(this);
}

GF_Particle::~GF_Particle()
{
    GF_Particle::currentPopulation--;
}

void GF_Particle::respawn()
{
    elapsedTime = 0;
    position.x = centerSpawnPos.x + pollenRadius*2*Maths::random();
    position.y = centerSpawnPos.y + pollenRadius*2*Maths::random();
    position.z = centerSpawnPos.z + pollenRadius*2*Maths::random();
    velocity.x = (Maths::random()-0.5f)*30;
    velocity.y = (Maths::random()-0.5f)*30;
    velocity.z = (Maths::random()-0.5f)*30;
    scale = Maths::random()*2.0f + 0.5f;
}

bool GF_Particle::update()
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

    return Global::levelId == LVL_GREEN_FOREST;
}
