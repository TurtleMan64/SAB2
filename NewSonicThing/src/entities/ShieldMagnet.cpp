#include <glad/glad.h>

#include "entity.h"
#include "../toolbox/vector.h"
#include "shieldmagnet.h"
#include "../engineTester/main.h"
#include "../entities/camera.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particlemaster.h"

#include <list>


ShieldMagnet::ShieldMagnet()
{
    visible = false;
}

void ShieldMagnet::step()
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
    
        ParticleMaster::createParticle(ParticleResources::textureSparkleLightBlue, &pos1, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);

        off.scale(-1);

        ParticleMaster::createParticle(ParticleResources::textureSparkleWhite, &pos2, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
    }
}

std::list<TexturedModel*>* ShieldMagnet::getModels()
{
    return nullptr;
}
