#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "beetle.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "dummy.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Beetle::modelsBody;
std::list<TexturedModel*> Beetle::modelsBlades;

float Beetle::hitRadius = 12.0f;

Beetle::Beetle()
{

}

Beetle::Beetle(float x, float y, float z, std::list<Entity*>* entityListToAdd)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	scale = 1;
	visible = true;
	updateTransformationMatrix();

	blades = new Dummy(&Beetle::modelsBlades); INCR_NEW("Entity");
	blades->setVisible(true);
	blades->setPosition(&position);
    updateBlades();
	entityListToAdd->push_back(blades);
}

void Beetle::updateBlades()
{
    //calculate new blades position
    Vector3f spinOff(-5.87751f, 0, 0);
    Vector3f yAxis(0, 1, 0);
    spinOff = Maths::rotatePoint(&spinOff, &yAxis, Maths::toRadians(rotY));
    blades->position = position + spinOff;
	blades->updateTransformationMatrix();
}

void Beetle::step()
{
	if (fabsf(position.x - Global::gameMainPlayer->position.x) < 100 &&
        fabsf(position.y - Global::gameMainPlayer->position.y) < 100 &&
        fabsf(position.z - Global::gameMainPlayer->position.z) < 100)
	{
        float xDiff = position.x - Global::gameMainPlayer->position.x;
        float yDiff = position.y - Global::gameMainPlayer->position.y;
        float zDiff = position.z - Global::gameMainPlayer->position.z;

        rotY = Maths::toDegrees(atan2f(zDiff, -xDiff));
        updateTransformationMatrix();

        extern float dt;

        blades->rotY += 360*dt;
        updateBlades();

		if ((xDiff*xDiff + yDiff*yDiff + zDiff*zDiff) < hitRadius*hitRadius)
		{
			if (Global::gameMainPlayer->isVulnerable())
			{
				Global::gameMainPlayer->takeDamage(&position);
			}
			else
			{
				die();
				Global::gameMainPlayer->rebound(&position);
			}
		}
	}
}

void Beetle::die()
{
	AudioPlayer::play(3, &position);

	//Global::gamePlayer->increaseCombo();

    //very strange bug that i cant figure out. sometimes when you homing attack a beetle and hit it, 
    // there will be a visible entity that returns a nullptr for their models during rendering.
    // im assuming that the entity is either the beetle or the blades. setting boths visibility to
    // false should fix the problem, but the source of the bug remains a mystery...
    // edit: this is because of the blades being in a different chunk than the one they were spawned in on,
    // because the blades change position when the beetle spins. i have edited the deleteChunkedEntity
    // function to deal with these special cases.
    //visible = false;
    //blades->visible = false;

	Main_deleteChunkedEntity(this);
	Main_deleteChunkedEntity(blades);

	//float height = 10.0f;
	//float spread = 20.0f;
    //
	//for (int i = 7; i != 0; i--)
	//{
	//	Vector3f pos(
	//		getX() + spread*(Maths::random() - 0.5f),
	//		getY() + spread*(Maths::random() - 0.5f) + height,
	//		getZ() + spread*(Maths::random() - 0.5f));
    //
	//	Vector3f vel(0, 0, 0);
    //
	//	new Particle(ParticleResources::textureExplosion1, &pos, &vel,
	//		0, 45, 0, 3 * Maths::random() + 6, 0, false);
	//}
    //
	//Vector3f pos(
	//	getX(),
	//	getY() + height,
	//	getZ());
    //
	//Vector3f vel(0, 0, 0);
	//
	//new Particle(ParticleResources::textureExplosion2, &pos, &vel,
	//	0, 55, 0, 20, 0, false);
    //
	//Global::gameScore += 100;
}

std::list<TexturedModel*>* Beetle::getModels()
{
	return &Beetle::modelsBody;
}

void Beetle::loadStaticModels()
{
	if (Beetle::modelsBody.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Beetle static models...\n");
	#endif

	loadModel(&Beetle::modelsBody,   "res/Models/Objects/Beetle/", "Beetle");
	loadModel(&Beetle::modelsBlades, "res/Models/Objects/Beetle/", "Blades");
}

void Beetle::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Beetle static models...\n");
	#endif

	Entity::deleteModels(&Beetle::modelsBody);
	Entity::deleteModels(&Beetle::modelsBlades);
}

const bool Beetle::canHomingAttackOn()
{
	return true;
}

const Vector3f Beetle::getHomingCenter()
{
	return position;
}

const bool Beetle::isEnemy()
{
	return true;
}
