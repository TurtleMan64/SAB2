/*
 TODO
 -Making the platform move
	-perhaps use a state machine?
	-add a displacement variable to track the distance moved, direction inspecific
	-0 is starting displacement, displacementMax is ending
	DONE
 -do the platform stop and switch directions
	DONE
 -make the platform shake when it stops
	DONE
 -Make the wheels properly rotate in the direction of movement
	-basically done, multiply by -1 in step when reversing direction of movement
	-done
 -Make sure collisions work properly
	-fix collisions on the front and back of the platfrom when moving
	-use two circles to make the collision box, easier to calculate than rectangles
	-move into place the same way as the wheels, then update positions as it moves
 -Is there a sound?
*/

#include "mhyellowmovingplatform.h"
#include "../entity.h"
#include "../camera.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"
#include "../car.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"
#include "../../animation/body.h"

#include <algorithm>

extern float dt;

std::list<TexturedModel*> MH_YellowMovingPlatform::models;
std::list<TexturedModel*> MH_YellowMovingPlatform::modelsWheelFront;
std::list<TexturedModel*> MH_YellowMovingPlatform::modelsWheelBack;
std::list<TexturedModel*> MH_YellowMovingPlatform::modelsTransparent;
CollisionModel* MH_YellowMovingPlatform::cmOriginal;

MH_YellowMovingPlatform::MH_YellowMovingPlatform(float x, float y, float z, int platformMovesOnXAxis, float displacementMax, float speed)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	rotZ = 0;

	this->wheelMovementDirectionMultiplier = 1;

	this->platformMovesOnXAxis = platformMovesOnXAxis;
    this->displacementMax = displacementMax;
	this->displacementCurrent = 0;
    this->speed = speed;

	directionVector = calculateDirectionVector();

	this->rotY = (this->platformMovesOnXAxis == true) ? 0 : 90;
	//rotate 180 if going in opposite direction (decided by negative displacementMax value)
	//also set the wheel movement direction multiplier to -1, so the wheels move in the right direction
	if (displacementMax < 0) 
	{
		rotY += 180;
		wheelMovementDirectionMultiplier = -1;
	}

	scale = 1;
	visible = true;

    positionInitial = &position;

	if (displacementMax == 0)
	{
		printf("ERROR! YellowMovingPlatform variable displacementMax has a value of 0, which is bad.");
	}

	this->platformMovesOnXAxis = platformMovesOnXAxis;
    this->displacementMax = displacementMax;
    this->speed = speed;

	stateCurrent = MOVING_FORWARD;

	setupModelWheelFront();
	setupModelWheelBack();
	setupModelTransparent();

	updateTransformationMatrix();

	collideModelOriginal = MH_YellowMovingPlatform::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/Objects/MetalHarbor/YellowMovingPlatform/", "YellowMovingPlatformCollision");

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void MH_YellowMovingPlatform::step() 
{
	Vector3f movementAmount;

	switch (stateCurrent)
	{
		case MOVING_FORWARD:
			movementAmount = calculateMovementAmount(directionVector);
			position = position + movementAmount;
			displacementCurrent += movementAmount.length();
			syncBodyPositionsRelative(movementAmount);
			spinWheels();

			if (collideModelTransformed->playerIsOn)
			{
				movePlayer(movementAmount);
			}
			
			if (fabs(displacementCurrent) >= fabs(displacementMax))
			{
				displacementCurrent = displacementMax;
				wheelMovementDirectionMultiplier *= -1;
				
				stateCurrent = STOPPED;
				positionStopped = position;
				stoppedTimer = 0;
				shakeTimer = SHAKE_TIMER_MIN;
			}
			break;				
		case MOVING_BACKWARDS:
			movementAmount = calculateMovementAmount(Vector3f() - directionVector);
			position = position + movementAmount;
			displacementCurrent -= movementAmount.length();
			syncBodyPositionsRelative(movementAmount);
			spinWheels();

			if (collideModelTransformed->playerIsOn)
			{
				movePlayer(movementAmount);
			}

			if (fabs(displacementCurrent <= 0))
			{
				displacementCurrent = 0;
				position = positionInitial;
				syncBodyPositionsAbsolute();
				wheelMovementDirectionMultiplier *= -1;
				
				stateCurrent = STOPPED;
				positionStopped = position;
				stoppedTimer = 0;
				shakeTimer = SHAKE_TIMER_MIN;
			}
			break;
		case STOPPED:
			stoppedTimer += dt;

			movementAmount = shakePlatform();
			position = position + movementAmount;
			syncBodyPositionsRelative(movementAmount);

			if (collideModelTransformed->playerIsOn)
			{
				movePlayer(movementAmount);
			}

			if (stoppedTimer > 2)
			{
				if (displacementCurrent == displacementMax)
				{
					stateCurrent = MOVING_BACKWARDS;
				}
				else
				{
					stateCurrent = MOVING_FORWARD;
				}
			}
			break;
	}

	updateTransformationMatrix();
	wheelFront->updateTransformationMatrix();
	wheelBack->updateTransformationMatrix();
	bodyTransparent->updateTransformationMatrix();
	updateCollisionModel();

	Global::gameMainVehicle->animate();
	Global::gameMainVehicle->refreshCamera();
}

std::list<TexturedModel*>* MH_YellowMovingPlatform::getModels()
{
	return &MH_YellowMovingPlatform::models;
}

void MH_YellowMovingPlatform::loadStaticModels()
{
	if (MH_YellowMovingPlatform::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading MH_YellowMovingPlatform static models...\n");
	#endif

	loadModel(&MH_YellowMovingPlatform::models, "res/Models/Objects/MetalHarbor/YellowMovingPlatform/", "YellowMovingPlatform");
	loadModel(&MH_YellowMovingPlatform::modelsWheelFront, "res/Models/Objects/MetalHarbor/YellowMovingPlatform/", "YellowMovingPlatformWheelFront");
	loadModel(&MH_YellowMovingPlatform::modelsWheelBack, "res/Models/Objects/MetalHarbor/YellowMovingPlatform/", "YellowMovingPlatformWheelBack");
    loadModel(&MH_YellowMovingPlatform::modelsTransparent, "res/Models/Objects/MetalHarbor/YellowMovingPlatform/", "YellowMovingPlatformTransparent");


	if (MH_YellowMovingPlatform::cmOriginal == nullptr)
	{
		MH_YellowMovingPlatform::cmOriginal = loadCollisionModel("Models/Objects/MetalHarbor/YellowMovingPlatform/", "YellowMovingPlatformCollision");
	}
}

void MH_YellowMovingPlatform::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting MH_YellowMovingPlatform static models...\n");
	#endif

	Entity::deleteModels(&MH_YellowMovingPlatform::models);
	Entity::deleteModels(&MH_YellowMovingPlatform::modelsWheelFront);
	Entity::deleteModels(&MH_YellowMovingPlatform::modelsWheelBack);
	Entity::deleteModels(&MH_YellowMovingPlatform::modelsTransparent);
	Entity::deleteCollisionModel(&MH_YellowMovingPlatform::cmOriginal);
}

Vector3f MH_YellowMovingPlatform::calculateDirectionVector()
{
	Vector3f directionVectorLocal = Vector3f();
	if (platformMovesOnXAxis)
	{
		directionVectorLocal.z = fabs(displacementMax) / displacementMax;
	}
	else
	{
		directionVectorLocal.x = fabs(displacementMax) / displacementMax;
	}
	return directionVectorLocal;
}

void MH_YellowMovingPlatform::setupModelWheelFront()
{
	wheelFront = new Body(&MH_YellowMovingPlatform::modelsWheelFront);
	wheelFront->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntityPass2(wheelFront);
	wheelFront->setPosition(&position);
	wheelFront->setRotY(rotY);
}

void MH_YellowMovingPlatform::setupModelWheelBack()
{
	wheelBack = new Body(&MH_YellowMovingPlatform::modelsWheelBack);
	wheelBack->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntityPass2(wheelBack);
	wheelBack->setPosition(&position);
	wheelBack->setRotY(rotY);
}

void MH_YellowMovingPlatform::setupModelTransparent()
{
	bodyTransparent = new Body(&MH_YellowMovingPlatform::modelsTransparent);
	bodyTransparent->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntityPass3(bodyTransparent);
	bodyTransparent->setRotY(rotY);
}

Vector3f MH_YellowMovingPlatform::calculateMovementAmount(Vector3f directionVectorLocal)
{
	//return Vector3f();
	return directionVectorLocal.scaleCopy(speed * dt);
}

void MH_YellowMovingPlatform::syncBodyPositionsRelative(Vector3f movementAmount)
{
	bodyTransparent->position = position;
	
	wheelFront->position = wheelFront->position + movementAmount;
	wheelBack->position = wheelBack->position + movementAmount;
}

void MH_YellowMovingPlatform::syncBodyPositionsAbsolute()
{
	bodyTransparent->position = position;

	wheelFront->position = position + directionVector.scaleCopy(WHEEL_OFFSET_FRONT_HORIZONTAL);
	wheelFront->position.y += WHEEL_OFFSET_FRONT_VERTICAL;

	wheelBack->position = position + directionVector.scaleCopy(WHEEL_OFFSET_BACK_HORIZONTAL);
	wheelBack->position.y += WHEEL_OFFSET_BACK_VERTICAL;
}

void MH_YellowMovingPlatform::spinWheels()
{
	wheelFront->rotX += WHEEL_SPEED_FRONT * wheelMovementDirectionMultiplier;
	wheelBack->rotX += WHEEL_SPEED_BACK * wheelMovementDirectionMultiplier;
}

void MH_YellowMovingPlatform::movePlayer(Vector3f movementAmount)
{
	Vector3f newPlayerPos = movementAmount + Global::gameMainVehicle->getPosition();
	Global::gameMainVehicle->position = newPlayerPos;
}

Vector3f MH_YellowMovingPlatform::shakePlatform()
{
	Vector3f distanceFromPositionStopped = positionStopped - position;

	if (shakeTimer > SHAKE_TIMER_MAX)
	{
		return distanceFromPositionStopped;
	}
	shakeTimer += dt * 30;
	return directionVector.scaleCopy(sinf(shakeTimer)/shakeTimer * 5) + distanceFromPositionStopped;
}