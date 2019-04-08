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
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../toolbox/input.h"

#include <algorithm>

extern float dt;

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
	handleVerticalDisplacementBottom = handleVerticalDisplacement;

	cameraDirectionVector = calculateCameraDirectionVector();

	setupPulleyRope();

	setupPulleyTop();
	
	pulleyTopYPosition = position.y;

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
	//Idea: make pulley bob when it hits the top? like SA2

	//Change all of this so the start moving is separate from the actual moving, isMoving variable required
    if (playerIsOnPulley)
	{
		if (!handleAtTop()) 
		{
			//Pulley not yet at top and player is riding, move towards top

			playPulleySound();

			movePulley(MOVE_UP);
		}
		else
		{
			//Make sure the pulley doesn't go any closer than the minimum
			handleVerticalDisplacement = HANDLE_VERTICAL_DISPLACEMENT_MINIMUM;

			stopPulleySound();
		}

		//Make player attach to the pulleys position and not move
		Global::gameMainVehicle->setPosition(position.x, position.y - 5, position.z);
		Global::gameMainVehicle->setVelocity(cameraDirectionVector.x, 0, cameraDirectionVector.z);

		if (jumpInputPressed()) //get off the pulley, should also happen if damaged
		{
			Global::gameMainVehicle->setVelocity(0,0,0);
			Global::gameMainVehicle->setVelocityMovesPlayer(true);
			Global::gameMainVehicle->setOnPulley(false);
			Global::gameMainVehicle->doJumpPulley(cameraDirectionVector);
			
			playerIsOnPulley = false;
		}
	}
	else if (playerWithinHandleHitbox() && handleAtBottom()) //Player gets on the handle
	{
		//Set the pulley to the state of having the player on it
		playerIsOnPulley = true;

		//This below is all just setting up player variables in preparation for the movement

		//Used for the animation of the player holding the pulley
		Global::gameMainVehicle->setOnPulley(true);
		//Make player unable to move so velocity only changes camera direction
		Global::gameMainVehicle->setVelocityMovesPlayer(false);
		//Make player face the right direction
		Global::gameMainVehicle->setRotY(rotY);
	}
	else if (!handleAtBottom())
	{
		//Pulley not yet at bottom and player not on, move towards bottom

		playPulleySound();

		movePulley(MOVE_DOWN);
	}
	else
	{
		//Make sure the pulley doesn't go any father than the maximum displacement
		handleVerticalDisplacement = handleVerticalDisplacementBottom;
		stopPulleySound();
	}

	position.y = pulleyTopYPosition - handleVerticalDisplacement;

	updateTransformationMatrix();
	//stretch the rope out to where the pulley is
	rope->updateTransformationMatrix(1, handleVerticalDisplacement, 1);
	Global::gameMainVehicle->animate();
	Global::gameMainVehicle->refreshCamera();
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

Vector3f Pulley::calculateCameraDirectionVector()
{
	return Vector3f(sin(Maths::toRadians(rotY)) * 1000, 0, -cos(Maths::toRadians(rotY)) * 1000);
}

void Pulley::setupPulleyRope()
{
    rope = new Body(&Pulley::modelsRope);
	rope->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntity(rope);
	rope->setPosition(&position);
}

void Pulley::setupPulleyTop()
{
    top = new Body(&Pulley::modelsTop);
	top->setVisible(true);
	INCR_NEW("Entity");
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
	return handleVerticalDisplacement >= handleVerticalDisplacementBottom;
}

bool Pulley::handleAtTop()
{
	return handleVerticalDisplacement <= HANDLE_VERTICAL_DISPLACEMENT_MINIMUM;
}

void Pulley::playPulleySound()
{
	if (pulleyAudioSource == nullptr)
	{
		pulleyAudioSource = AudioPlayer::play(57, top->getPosition(), 1, false);
	}
	pulleyAudioSource->setLooping(true);
}

void Pulley::stopPulleySound()
{
	if (pulleyAudioSource != nullptr)
	{
		if (pulleyAudioSource->isPlaying())
		{
			pulleyAudioSource->stop();
			pulleyAudioSource = nullptr;
		}
	}
}

void Pulley::movePulley(bool movePulleyUp)
{
	//if true, the pulley moves up, if false, it moves down
	
	//is either 1 or -1, used to control what direction the pulley moves
	int pulleyDirection = getPulleyMoveDirection(movePulleyUp);

	handleVerticalDisplacement += PULLEY_SPEED * pulleyDirection * dt;
}

int Pulley::getPulleyMoveDirection(bool movePulleyUp)
{
	if (movePulleyUp)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

bool Pulley::jumpInputPressed()
{
	return Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1;
}