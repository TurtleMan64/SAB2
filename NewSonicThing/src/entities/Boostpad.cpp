#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "boostpad.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/car.h"
#include "../toolbox/maths.h"
#include "dummy.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../collision/collisionchecker.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Boostpad::modelsPad;
std::list<TexturedModel*> Boostpad::modelsBolt;
CollisionModel* Boostpad::cmOriginal;

Boostpad::Boostpad()
{

}

Boostpad::Boostpad(
	float x,     float y,     float z, 
	float normX, float normY, float normZ,
	float atX,   float atY,   float atZ)
{
	position.x = x;
	position.y = y;
	position.z = z;
	scale = 1;
	visible = true;
	playerIsIn = false;

	bolt = new Dummy(&Boostpad::modelsBolt); INCR_NEW("Entity");
	bolt->setVisible(true);
	bolt->setPosition(&position);
	Main_addTransparentEntity(bolt);


	Vector3f currNorm(normX, normY, normZ);
	Vector3f groundSpeeds = Maths::calculatePlaneSpeed(atX, atY, atZ, &currNorm);
	float twistAngle = Maths::toDegrees(atan2f(-groundSpeeds.z, groundSpeeds.x));
	float nX = currNorm.x;
	float nY = currNorm.y;
	float nZ = currNorm.z;
	float normHLength = sqrtf(nX*nX + nZ*nZ);
	float pitchAngle = Maths::toDegrees(atan2f(nY, normHLength));
	float yawAngle = Maths::toDegrees(atan2f(-nZ, nX));
	float diff = Maths::compareTwoAngles(twistAngle, yawAngle);

	rotX = diff;
	rotY = yawAngle;
	rotZ = pitchAngle;
	rotRoll = 0;
	updateTransformationMatrix();

	bolt->setRotation(rotX, rotY, rotZ, 0);
	bolt->updateTransformationMatrix();


	//cmTransformed = loadCollisionModel("Models/Boostpad/", "Collision");
	//CollisionChecker::addCollideModel(cmTransformed);
	//updateCollisionModel();
}

void Boostpad::step()
{
	if (abs(getX() - Global::gameCamera->eye.x) > ENTITY_RENDER_DIST)
	{
		setVisible(false);
		bolt->setVisible(false);
	}
	else
	{
		if (abs(getZ() - Global::gameCamera->eye.z) > ENTITY_RENDER_DIST)
		{
			setVisible(false);
			bolt->setVisible(false);
		}
		else
		{
			setVisible(true);
			bolt->setVisible(true);

			if (abs(getX() - Global::gameMainVehicle->getPosition()->x) < 10 &&
				abs(getZ() - Global::gameMainVehicle->getPosition()->z) < 10 &&
				abs(getY() - Global::gameMainVehicle->getPosition()->y) < 10)
			{
				Vector3f diff(
					getX()-Global::gameMainVehicle->getPosition()->x, 
					getY()-Global::gameMainVehicle->getPosition()->y,
					getZ()-Global::gameMainVehicle->getPosition()->z);

				if (diff.lengthSquared() < 5.5f*5.5f)
				{
					if (!playerIsIn)
					{
						AudioPlayer::play(0, getPosition());
						//Global::gameMainVehicle->giveMeABoost();
					}

					playerIsIn = true;
				}
				else
				{
					playerIsIn = false;
				}
			}
			else
			{
				playerIsIn = false;
			}
		}
	}
}

std::list<TexturedModel*>* Boostpad::getModels()
{
	return &Boostpad::modelsPad;
}

void Boostpad::loadStaticModels()
{
	if (Boostpad::modelsPad.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Boostpad static models...\n");
	#endif

	loadModel(&Boostpad::modelsPad,  "res/Models/Misc/Boostpad/", "Pad");
	loadModel(&Boostpad::modelsBolt, "res/Models/Misc/Boostpad/", "Bolt");

	if (Boostpad::cmOriginal == nullptr)
	{
		//Boostpad::cmOriginal = loadCollisionModel("Models/Boostpad/", "Collision");
	}
}

void Boostpad::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Boostpad static models...\n");
	#endif

	Entity::deleteModels(&Boostpad::modelsPad);
	Entity::deleteModels(&Boostpad::modelsBolt);
	//Entity::deleteCollisionModel(&Boostpad::cmOriginal);
}
