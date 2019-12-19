#include <glad/glad.h>

#include "entity.h"
#include "../toolbox/vector.h"
#include "shieldgreen.h"
#include "../engineTester/main.h"
#include "../entities/camera.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particlemaster.h"

#include <list>

ShieldGreen::ShieldGreen()
{
    visible = false;
}

void ShieldGreen::step()
{
    setPosition(Global::gameMainPlayer->getPosition());
    updateTransformationMatrix();

    Vector3f center = Global::gameMainPlayer->getCenterPosition();

    for (int i = 0; i < 10; i++)
    {
        Vector3f off = Maths::randomPointOnSphere();
        off.scale(8);

        Vector3f pos1 = center + off;
        Vector3f pos2 = center - off;

        off.scale((1/16.0f)*60);
    
        ParticleMaster::createParticle(ParticleResources::textureSparkleGreen, &pos1, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f, true);

        off.scale(-1);

        ParticleMaster::createParticle(ParticleResources::textureSparkleGreen, &pos2, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f, true);
    }
}

std::list<TexturedModel*>* ShieldGreen::getModels()
{
    return nullptr;
}
