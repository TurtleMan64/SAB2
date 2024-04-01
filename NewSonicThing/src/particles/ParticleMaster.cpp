#include "particlemaster.hpp"
#include "particle.hpp"
#include "particlerenderer.hpp"
#include "particleshader.hpp"
#include "particletexture.hpp"
#include "../toolbox/matrix.hpp"
#include "../entities/camera.hpp"
#include "insertionsort.hpp"
#include "../engineTester/main.hpp"
#include "../entities/GreenForest/gfparticle.hpp"
#include "../entities/IceCap/icparticle.hpp"
#include "../entities/CastleTown/caparticle.hpp"

#include <unordered_map>
#include <list>

std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>*> ParticleMaster::particlesStandard;
std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*> ParticleMaster::particlesGF;
std::unordered_map<ParticleTexture*, std::list<IC_Particle*>*> ParticleMaster::particlesIC;
std::unordered_map<ParticleTexture*, std::list<CA_Particle*>*> ParticleMaster::particlesCA;
ParticleRenderer* ParticleMaster::renderer = nullptr;

void ParticleMaster::init(Matrix4f* projectionMatrix)
{
    ParticleMaster::renderer = new ParticleRenderer(projectionMatrix); INCR_NEW("ParticleRenderer");
}

void ParticleMaster::update(Camera* /*cam*/)
{
    ANALYSIS_START("Particle Updating");
    //standard
    {
        std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>*>::iterator mapIt = ParticleMaster::particlesStandard.begin();
        //bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesStandard.end())
        {
            std::list<ParticleStandard*>* list = mapIt->second;

            std::list<ParticleStandard*>::iterator it = list->begin();

            //deletedEntry = false;

            while (it != list->end())
            {
                ParticleStandard* p = *it;

                bool stillAlive = p->update();
                if (stillAlive == false)
                {
                    it = list->erase(it);
                    delete p; INCR_DEL("ParticleStandard");

                    //if (list->size() == 0)
                    {
                        //mapIt = ParticleMaster::particlesStandard.erase(mapIt);
                        //deletedEntry = true;
                        //break;
                    }
                }
                else
                {
                    it++;
                }
            }

            //if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }

    //GF
    {
        std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*>::iterator mapIt = ParticleMaster::particlesGF.begin();
        //bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesGF.end())
        {
            std::list<GF_Particle*>* list = mapIt->second;

            std::list<GF_Particle*>::iterator it = list->begin();

            //deletedEntry = false;

            while (it != list->end())
            {
                GF_Particle* p = *it;

                bool stillAlive = p->update();
                if (stillAlive == false)
                {
                    it = list->erase(it);
                    delete p; INCR_DEL("GF_Particle");

                    //if (list->size() == 0)
                    {
                        //mapIt = ParticleMaster::particlesGF.erase(mapIt);
                        //deletedEntry = true;
                        //break;
                    }
                }
                else
                {
                    it++;
                }
            }

            //if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }

    //IC
    {
        std::unordered_map<ParticleTexture*, std::list<IC_Particle*>*>::iterator mapIt = ParticleMaster::particlesIC.begin();
        //bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesIC.end())
        {
            std::list<IC_Particle*>* list = mapIt->second;

            std::list<IC_Particle*>::iterator it = list->begin();

            //deletedEntry = false;

            while (it != list->end())
            {
                IC_Particle* p = *it;

                bool stillAlive = p->update();
                if (stillAlive == false)
                {
                    it = list->erase(it);
                    delete p; INCR_DEL("IC_Particle");

                    //if (list->size() == 0)
                    {
                        //mapIt = ParticleMaster::particlesIC.erase(mapIt);
                        //deletedEntry = true;
                        //break;
                    }
                }
                else
                {
                    it++;
                }
            }

            //if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }

    //CA
    {
        std::unordered_map<ParticleTexture*, std::list<CA_Particle*>*>::iterator mapIt = ParticleMaster::particlesCA.begin();
        //bool deletedEntry = false;

        while (mapIt != ParticleMaster::particlesCA.end())
        {
            std::list<CA_Particle*>* list = mapIt->second;

            std::list<CA_Particle*>::iterator it = list->begin();

            //deletedEntry = false;

            while (it != list->end())
            {
                CA_Particle* p = *it;

                bool stillAlive = p->update();
                if (stillAlive == false)
                {
                    it = list->erase(it);
                    delete p; INCR_DEL("CA_Particle");

                    //if (list->size() == 0)
                    {
                        //mapIt = ParticleMaster::particlesCC.erase(mapIt);
                        //deletedEntry = true;
                        //break;
                    }
                }
                else
                {
                    it++;
                }
            }

            //if (deletedEntry == false)
            {
                mapIt++;
            }
        }
    }

    ANALYSIS_DONE("Particle Updating");
}

void ParticleMaster::renderParticles(Camera* camera, float brightness, int clipSide)
{
    ANALYSIS_START("Particle Rendering");
    ParticleMaster::renderer->render(
        &ParticleMaster::particlesStandard,
        &ParticleMaster::particlesGF,
        &ParticleMaster::particlesIC,
        &ParticleMaster::particlesCA,
        camera, brightness, clipSide);
    ANALYSIS_DONE("Particle Rendering");
}

void ParticleMaster::cleanUp()
{
    ParticleMaster::renderer->cleanUp();
}

void ParticleMaster::addParticleStandard(ParticleStandard* particle)
{
    std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>*>::iterator it;
    it = ParticleMaster::particlesStandard.find(particle->texture);

    std::list<ParticleStandard*>* list = nullptr;

    if (it == ParticleMaster::particlesStandard.end())
    {
        list = new std::list<ParticleStandard*>(); INCR_NEW("std::list<ParticleStandard*>");
        ParticleMaster::particlesStandard[particle->texture] = list;
    }
    else
    {
        list = it->second;
    }

    list->push_back(particle);
}

void ParticleMaster::addParticleGF(GF_Particle* particle)
{
    std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*>::iterator it;
    it = ParticleMaster::particlesGF.find(particle->texture);

    std::list<GF_Particle*>* list = nullptr;

    if (it == ParticleMaster::particlesGF.end())
    {
        list = new std::list<GF_Particle*>(); INCR_NEW("std::list<GF_Particle*>");
        ParticleMaster::particlesGF[particle->texture] = list;
    }
    else
    {
        list = it->second;
    }

    list->push_back(particle);
}

void ParticleMaster::addParticleIC(IC_Particle* particle)
{
    std::unordered_map<ParticleTexture*, std::list<IC_Particle*>*>::iterator it;
    it = ParticleMaster::particlesIC.find(particle->texture);

    std::list<IC_Particle*>* list = nullptr;

    if (it == ParticleMaster::particlesIC.end())
    {
        list = new std::list<IC_Particle*>(); INCR_NEW("std::list<IC_Particle*>");
        ParticleMaster::particlesIC[particle->texture] = list;
    }
    else
    {
        list = it->second;
    }

    list->push_back(particle);
}

void ParticleMaster::addParticleCA(CA_Particle* particle)
{
    std::unordered_map<ParticleTexture*, std::list<CA_Particle*>*>::iterator it;
    it = ParticleMaster::particlesCA.find(particle->texture);

    std::list<CA_Particle*>* list = nullptr;

    if (it == ParticleMaster::particlesCA.end())
    {
        list = new std::list<CA_Particle*>(); INCR_NEW("std::list<CA_Particle*>");
        ParticleMaster::particlesCA[particle->texture] = list;
    }
    else
    {
        list = it->second;
    }

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
    float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, velocity, gravityEffect, lifeLength, scale, scaleChange, posIsRef, onlyRendersOnce, opacity); INCR_NEW("ParticleStandard");
    }
}

void ParticleMaster::createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
    float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, bool optional)
{
    if (Global::renderParticles || !optional)
    {
        new ParticleStandard(texture, position, velocity, gravityEffect, lifeLength, scale, scaleChange, posIsRef, onlyRendersOnce); INCR_NEW("ParticleStandard");
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
    //Standard particles
    {
        for (auto particlesMapEntry : ParticleMaster::particlesStandard)
        {
            std::list<ParticleStandard*>* particles = particlesMapEntry.second;

            for (auto it = particles->begin(); it != particles->end(); it++)
            {
                delete *it; INCR_DEL("ParticleStandard");
            }

            particles->clear(); // clear the list, but dont clear the texture->list entry.
        }
    }

    //GF
    {
        for (auto particlesMapEntry : ParticleMaster::particlesGF)
        {
            std::list<GF_Particle*>* particles = particlesMapEntry.second;

            for (auto it = particles->begin(); it != particles->end(); it++)
            {
                delete *it; INCR_DEL("GF_Particle");
            }

            particles->clear(); // clear the list, but dont clear the texture->list entry.
        }
    }

    //IC
    {
        for (auto particlesMapEntry : ParticleMaster::particlesIC)
        {
            std::list<IC_Particle*>* particles = particlesMapEntry.second;

            for (auto it = particles->begin(); it != particles->end(); it++)
            {
                delete* it; INCR_DEL("IC_Particle");
            }

            particles->clear(); // clear the list, but dont clear the texture->list entry.
        }
    }

    //CA
    {
        for (auto particlesMapEntry : ParticleMaster::particlesCA)
        {
            std::list<CA_Particle*>* particles = particlesMapEntry.second;

            for (auto it = particles->begin(); it != particles->end(); it++)
            {
                delete* it; INCR_DEL("CA_Particle");
            }

            particles->clear(); // clear the list, but dont clear the texture->list entry.
        }
    }
}
