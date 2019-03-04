#include "pulley.h"
#include "entity.h"
#include "camera.h"
#include "../engineTester/main.h"
#include "../models/models.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../toolbox/maths.h"
#include "../collision/collisionmodel.h"
#include "../collision/collisionchecker.h"
#include "../animation/body.h"
#include "../entities/car.h"

#include <algorithm>

std::list<TexturedModel*> Pulley::modelsHandle;
std::list<TexturedModel*> Pulley::modelsRope;
std::list<TexturedModel*> Pulley::modelsTop;
CollisionModel* Pulley::cmTop;

Pulley::Pulley() {}

Pulley::Pulley(float x, float y, float z, float newRotY, float handleVerticalDisplacement)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	rotY = newRotY;
	rotZ = 0;

	scale = 1;
	visible = true;

    this->handleVerticalDisplacement = handleVerticalDisplacement;

	setupPulleyRope();

	setupPulleyTop();
	
	//Move pulley down to initial position
	position.y -= handleVerticalDisplacement;

	updateTransformationMatrix();
	//stretch the rope out to where the pulley is
	rope->updateTransformationMatrix(1, handleVerticalDisplacement, 1);
	//the top never moves or changes so it won't be updated again.
	top->updateTransformationMatrix();
	cmTop->transformModel(collideModelTransformed, top->getPosition());
}

void Pulley::step() 
{
	//Change all of this so the start moving is separate from the actual moving, isMoving variable required
    if (playerIsOnPulley)
	{
		if (!handleAtTop()) 
		{
			//Pulley not yet at top and player is riding, move towards top

			playPulleySound();

			movePulley(true);

			//Make player unable to move so velocity only changes camera direction
			Global::gameMainVehicle->setVelocityMovesPlayer(false);
			//Used for the animation of the player holding the pulley
			Global::gameMainVehicle->setOnPulley(true);
		}

		if (jumpInputPressed()) //get off the pulley, should also happen if damaged
		{
			Global::gameMainVehicle->setVelocityMovesPlayer(true);
			Global::gameMainVehicle->setOnPulley(false);
			
			playerIsOnPulley = false;

			//TODO: figure out how to make the player jump off and have a homing attack and such
		}
	}
	else if (playerWithinHandleHitbox() && handleAtBottom()) //Player gets on the handle
	{
		//get on the pulley
		playerIsOnPulley = true;

	}
	else if (!handleAtBottom)
	{
		//Pulley not yet at bottom and player not on, move towards bottom

		playPulleySound();

		movePulley(false);
	}
	updateTransformationMatrix();
	//stretch the rope out to where the pulley is
	rope->updateTransformationMatrix(1, handleVerticalDisplacement, 1);
	Global::gameMainVehicle->animate();
}

std::list<TexturedModel*>* Pulley::getModels()
{
	return &Pulley::modelsHandle;
}

void Pulley::loadStaticModels()
{
	if (Pulley::modelsHandle.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Pulley static models...\n");
	#endif

	loadModel(&Pulley::modelsHandle, "res/Models/Objects/Pulley/", "PulleyHandle");
	loadModel(&Pulley::modelsRope,   "res/Models/Objects/Pulley/", "PulleyRope");
	loadModel(&Pulley::modelsTop,    "res/Models/Objects/Pulley/", "PulleyTop");

	if (Pulley::cmTop == nullptr)
	{
		Pulley::cmTop = loadCollisionModel("Models/Objects/Pulley/", "PulleyTopCollision");
	}
}

void Pulley::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Pulley static models...\n");
	#endif

	Entity::deleteModels(&Pulley::modelsHandle);
	Entity::deleteModels(&Pulley::modelsRope);
	Entity::deleteModels(&Pulley::modelsTop);
}

void Pulley::setupPulleyRope()
{
    rope = new Body(&Pulley::modelsRope);
	rope->setVisible(true);
	INCR_NEW
	Main_addEntity(rope);
	rope->setPosition(&position);
}

void Pulley::setupPulleyTop()
{
    top = new Body(&Pulley::modelsTop);
	top->setVisible(true);
	INCR_NEW
	Main_addEntity(top);
	top->setPosition(&position);

	collideModelOriginal = Pulley::cmTop;
	collideModelTransformed = loadCollisionModel("Models/Objects/Pulley/", "PulleyTopCollision");
	CollisionChecker::addCollideModel(collideModelTransformed);
	updateCollisionModel();
}

bool Pulley::playerWithinHandleHitbox()
{
	Vector3f playerPosition = Global::gameMainVehicle->getPosition();
	Vector3f playerPulleyDistance = playerPosition - position;
	float playerPulleyDistanceHorizontalSquared = pow(playerPulleyDistance.x, 2) + 
														pow(playerPulleyDistance.z, 2);
	return (playerPulleyDistanceHorizontalSquared <= powf(HITBOX_RADIUS, 2) &&
			fabsf(playerPulleyDistance.y) <= HITBOX_HEIGHT);
}

bool Pulley::handleAtBottom()
{
	return handleVerticalDisplacement <= handleVerticalDisplacementBottom;
}

bool Pulley::handleAtTop()
{
	return handleVerticalDisplacement >= HANDLE_VERTICAL_DISPLACEMENT_MINIMUM;
}

void Pulley::playPulleySound()
{

}

void Pulley::movePulley(bool movePulleyUp)
{
	//if true, the pulley moves up, if false, it moves down
}

bool Pulley::jumpInputPressed()
{
	//check for input somehow
}