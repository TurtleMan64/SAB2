#include <glad/glad.h>

#include "entity.hpp"
#include "../toolbox/vector.hpp"
#include "shieldmagnet.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particlemaster.hpp"

#include <list>


ShieldMagnet::ShieldMagnet()
{
    visible = false;
}

void ShieldMagnet::step()
{
    setPosition(Global::gameMainPlayer->getPosition());
    //updateTransformationMatrix();

    Vector3f center = Global::gameMainPlayer->getCenterPosition();

    for (int i = 0; i < 10; i++)
    {
        Vector3f off = Maths::randomPointOnSphere();
        off.scale(8);

        Vector3f pos1 = center + off;
        Vector3f pos2 = center - off;

        off.scale((1/16.0f)*60);
    
        ParticleMaster::createParticle(ParticleResources::textureSparkleLightBlue, &pos1, &off, 0, 0.0833f, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f, true);

        off.scale(-1);

        ParticleMaster::createParticle(ParticleResources::textureSparkleWhite, &pos2, &off, 0, 0.0833f, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f, true);
    }
}

std::vector<TexturedModel*>* ShieldMagnet::getModels()
{
    return nullptr;
}
