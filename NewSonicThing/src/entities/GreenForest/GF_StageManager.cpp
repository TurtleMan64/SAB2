#include <glad/glad.h>

#include "../entity.h"
#include "gfstagemanager.h"
#include "../../engineTester/main.h"
#include "../dummy.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../camera.h"
#include "../../toolbox/maths.h"
#include "../../particles/particle.h"
#include "../../particles/particleresources.h"

std::list<TexturedModel*> GF_StageManager::modelsSkydome;

GF_StageManager::GF_StageManager()
{
	visible = true;
}

void GF_StageManager::step()
{
	//setting the global water heights
	if (Global::gameMainPlayer->position.x < 25 &&
		Global::gameMainPlayer->position.z > 1800)
	{
		Global::waterHeight = -10000.0f;
	}
	else if (Global::gameMainPlayer->position.x < 143 &&
		     Global::gameMainPlayer->position.z > 2500 &&
		     Global::gameMainPlayer->position.y < -1041)
	{
		Global::waterHeight = -10000.0f;
	}
	else if (Global::gameMainPlayer->position.x < 447 &&
		     Global::gameMainPlayer->position.z > 2849)
	{
		Global::waterHeight = -10000.0f;
	}
	else if (Global::gameMainPlayer->position.x < 1058 &&
		     Global::gameMainPlayer->position.z > 2912)
	{
		Global::waterHeight = -10000.0f;
	}
	else if (Global::gameMainPlayer->position.z > 5240.0f)
	{
		Global::waterHeight = -2325.0f;
	}
	else
	{
		Global::waterHeight = -980.0f;
	}

	//set the position of the background sky dome
	position.y = Global::waterHeight+1000.0f;
	position.x = Global::gameMainPlayer->getX();
	position.z = Global::gameMainPlayer->getZ();
	updateTransformationMatrix();


	//generate pollen particles
	Vector3f center = Global::gameMainPlayer->position;
	center = center + Global::gameMainPlayer->vel.scaleCopy(0.4f);
	float radius2 = pollenRadius*2;
	float basex = center.x - pollenRadius;
	float basey = center.y - pollenRadius;
	float basez = center.z - pollenRadius;

	extern float dt;
	pollenSpawnTimer += dt;
	while (pollenSpawnTimer > 0)
	{
		pollenSpawnTimer -= pollenSparseness;

		Vector3f pos(basex + radius2*Maths::random(),
					 basey + radius2*Maths::random(),
					 basez + radius2*Maths::random());

		Vector3f vel((Maths::random()-0.5f)*30, (Maths::random()-0.5f)*30, (Maths::random()-0.5f)*30);

		const float lifeLength = 4.0f;
		const float scaleInitial = Maths::random()*2.0f + 0.5f;
		const float scaleChange = -0.1f;

		new Particle(ParticleResources::texturePollen, &pos, &vel, 0.0f, lifeLength, 0.0f, scaleInitial, scaleChange, false, false, 1.0f);
	}
}

std::list<TexturedModel*>* GF_StageManager::getModels()
{
	return &GF_StageManager::modelsSkydome;
}

void GF_StageManager::loadStaticModels()
{
	if (GF_StageManager::modelsSkydome.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading GF_StageManager static models...\n");
	#endif

	loadModel(&GF_StageManager::modelsSkydome,  "res/Models/Levels/GreenForest/", "Skydome");
}

void GF_StageManager::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting GF_StageManager static models...\n");
	#endif

	Entity::deleteModels(&GF_StageManager::modelsSkydome);
}
