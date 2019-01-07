#include "particlemaster.h"
#include "particle.h"
#include "particlerenderer.h"
#include "particleshader.h"
#include "particletexture.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "insertionsort.h"
#include "../engineTester/main.h"

#include <unordered_map>
#include <list>

std::unordered_map<ParticleTexture*, std::list<Particle*>> ParticleMaster::particles;
ParticleRenderer* ParticleMaster::renderer = nullptr;

void ParticleMaster::init(Matrix4f* projectionMatrix)
{
	ParticleMaster::renderer = new ParticleRenderer(projectionMatrix); INCR_NEW
}

//There was a crash in here once for some reason... something with 'list'?
void ParticleMaster::update(Camera* cam)
{
	std::unordered_map<ParticleTexture*, std::list<Particle*>>::iterator mapIt = ParticleMaster::particles.begin();
	bool deletedEntry = false;

	while (mapIt != ParticleMaster::particles.end())
	{
		std::list<Particle*>* list = &mapIt->second;

		std::list<Particle*>::iterator it = list->begin();

		deletedEntry = false;

		while (it != list->end())
		{
			Particle* p = *it;

			bool stillAlive = p->update(cam);
			if (stillAlive == false)
			{
				delete p; INCR_DEL //maybe switch this with next line?
				it = list->erase(it);

				if (list->size() == 0)
				{
					mapIt = ParticleMaster::particles.erase(mapIt);
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
			//InsertionSort::sortHighToLow(list); //Maybe remove this for debug?
			mapIt++;
		}
	}
	/*
	for (auto entry : particles)
	{
		std::list<Particle*>* list = &entry.second;

		for (Particle* particle : (*list))
		{
			bool stillAlive = particle->update(cam);
			if (stillAlive == false)
			{

			}
		}

		InsertionSort::sortHighToLow(list);
	}*/
}

void ParticleMaster::renderParticles(Camera* camera, float brightness, int clipSide)
{
	ParticleMaster::renderer->render(&ParticleMaster::particles, camera, brightness, clipSide);
}

void ParticleMaster::cleanUp()
{
	ParticleMaster::renderer->cleanUp();
}

void ParticleMaster::addParticle(Particle* particle)
{
	std::list<Particle*>* list = &ParticleMaster::particles[particle->getTexture()];
	list->push_back(particle);
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
	std::unordered_map<ParticleTexture*, std::list<Particle*>>::iterator mapIt = ParticleMaster::particles.begin();
	bool deletedEntry = false;

	while (mapIt != ParticleMaster::particles.end())
	{
		std::list<Particle*>* list = &mapIt->second;

		std::list<Particle*>::iterator it = list->begin();

		deletedEntry = false;

		while (it != list->end())
		{
			Particle* p = *it;

			delete p; INCR_DEL
			it = list->erase(it);

			if (list->size() == 0)
			{
				mapIt = ParticleMaster::particles.erase(mapIt);
				deletedEntry = true;
			}
		}

		if (deletedEntry == false)
		{
			mapIt++;
		}
	}
}
