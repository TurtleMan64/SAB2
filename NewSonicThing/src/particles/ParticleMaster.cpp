#include "particlemaster.h"
#include "particle.h"
#include "particlerenderer.h"
#include "particleshader.h"
#include "particletexture.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "insertionsort.h"
#include "../engineTester/main.h"
#include "../entities/GreenForest/gfparticle.h"

#include <unordered_map>
#include <list>

std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>> ParticleMaster::particlesStandard;
std::unordered_map<ParticleTexture*, std::list<GF_Particle*>> ParticleMaster::particlesGF;
ParticleRenderer* ParticleMaster::renderer = nullptr;

void ParticleMaster::init(Matrix4f* projectionMatrix)
{
    ParticleMaster::renderer = new ParticleRenderer(projectionMatrix); INCR_NEW("ParticleRenderer");
}

void ParticleMaster::update(Camera* /*cam*/)
{
    //standard
    {
        std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>>::iterator mapIt = ParticleMaster::particlesStandard.begin();
        bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesStandard.end())
        {
            std::list<ParticleStandard*>* list = &mapIt->second;

            std::list<ParticleStandard*>::iterator it = list->begin();

            deletedEntry = false;

            while (!deletedEntry && it != list->end())
            {
                ParticleStandard* p = *it;

                bool stillAlive = p->update();
                if (stillAlive == false)
                {
                    it = list->erase(it);
                    delete p; INCR_DEL("ParticleStandard");

                    if (list->size() == 0)
                    {
                        mapIt = ParticleMaster::particlesStandard.erase(mapIt);
                        deletedEntry = true;
                    }
                }
                else
                {
                    it++;
                }
            }

            if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }

    //GF
    {
        std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>::iterator mapIt = ParticleMaster::particlesGF.begin();
        bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesGF.end())
        {
            std::list<GF_Particle*>* list = &mapIt->second;

            std::list<GF_Particle*>::iterator it = list->begin();

            deletedEntry = false;

            while (!deletedEntry && it != list->end())
            {
                GF_Particle* p = *it;

                bool stillAlive = p->update();
                if (stillAlive == false)
                {
                    it = list->erase(it);
                    delete p; INCR_DEL("GF_Particle");

                    if (list->size() == 0)
                    {
                        mapIt = ParticleMaster::particlesGF.erase(mapIt);
                        deletedEntry = true;
                    }
                }
                else
                {
                    it++;
                }
            }

            if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }
}

void ParticleMaster::renderParticles(Camera* camera, float brightness, int clipSide)
{
    ParticleMaster::renderer->render(
        &ParticleMaster::particlesStandard,
        &ParticleMaster::particlesGF,
        camera, brightness, clipSide);
}

void ParticleMaster::cleanUp()
{
    ParticleMaster::renderer->cleanUp();
}

void ParticleMaster::addParticleStandard(ParticleStandard* particle)
{
    std::list<ParticleStandard*>* list = &ParticleMaster::particlesStandard[particle->getTexture()];
    list->push_back(particle);
}

void ParticleMaster::addParticleGF(GF_Particle* particle)
{
    std::list<GF_Particle*>* list = &ParticleMaster::particlesGF[particle->getTexture()];
    list->push_back(particle);
}

void ParticleMaster::createParticle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, lifeLength, scale, onlyRendersOnce); INCR_NEW("ParticleStandard");
    }
}

void ParticleMaster::createParticle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, lifeLength, scale, opacity, onlyRendersOnce); INCR_NEW("ParticleStandard");
    }
}

void ParticleMaster::createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, velocity, lifeLength, scale, onlyRendersOnce); INCR_NEW("ParticleStandard");
    }
}

void ParticleMaster::createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    float lifeLength, float rotation, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, velocity, gravityEffect, lifeLength, rotation, scale, scaleChange, posIsRef, onlyRendersOnce, opacity); INCR_NEW("ParticleStandard");
    }
}

void ParticleMaster::createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    float lifeLength, float rotation, float scaleX, float scaleXChange, float scaleY, float scaleYChange, 
    bool posIsRef, bool onlyRendersOnce, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, velocity, gravityEffect, lifeLength, rotation, scaleX, scaleXChange, scaleY, scaleYChange, posIsRef, onlyRendersOnce); INCR_NEW("ParticleStandard");
    }
}

void ParticleMaster::updateProjectionMatrix(Matrix4f* projectionMatrix)
{
    if (ParticleMaster::renderer != nullptr)
    {
        ParticleMaster::renderer->updateProjectionMatrix(projectionMatrix);
    }
}

void ParticleMaster::deleteAllParticles()
{
    //Standard
    {
        std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>>::iterator mapIt = ParticleMaster::particlesStandard.begin();
        bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesStandard.end())
        {
            std::list<ParticleStandard*>* list = &mapIt->second;

            std::list<ParticleStandard*>::iterator it = list->begin();

            deletedEntry = false;

            while (it != list->end())
            {
                ParticleStandard* p = *it;

                delete p; INCR_DEL("ParticleStandard");
                it = list->erase(it);

                if (list->size() == 0)
                {
                    mapIt = ParticleMaster::particlesStandard.erase(mapIt);
                    deletedEntry = true;
                }
            }

            if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }

    //GF
    {
        std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>::iterator mapIt = ParticleMaster::particlesGF.begin();
        bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesGF.end())
        {
            std::list<GF_Particle*>* list = &mapIt->second;

            std::list<GF_Particle*>::iterator it = list->begin();

            deletedEntry = false;

            while (it != list->end())
            {
                GF_Particle* p = *it;

                delete p; INCR_DEL("GF_Particle");
                it = list->erase(it);

                if (list->size() == 0)
                {
                    mapIt = ParticleMaster::particlesGF.erase(mapIt);
                    deletedEntry = true;
                }
            }

            if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }
}
