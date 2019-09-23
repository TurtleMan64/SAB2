/*
 TODO
 -Making the platform move
	-perhaps use a state machine?
 -Make the wheels properly rotate in the direction of movement
	-basically done, multiply by -1 in step when reversing direction of movement
 -Make sure collisions work properly
*/

#include "mhyellowmovingplatform.h"
#include "../entity.h"
#include "../camera.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"
#include "../../animation/body.h"

#include <algorithm>

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
    this->speed = speed;

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

	percentOfPathCompleted = 0;
	isMovingForward = true;

    positionInitial = &position;

	if (displacementMax == 0)
	{
		printf("ERROR! YellowMovingPlatform variable displacementMax has a value of 0, which is bad.");
	}

	this->platformMovesOnXAxis = platformMovesOnXAxis;
    this->displacementMax = displacementMax;
    this->speed = speed;

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
	wheelFront->rotX += wheelSpeedFront * wheelMovementDirectionMultiplier;
	wheelBack->rotX += wheelSpeedBack * wheelMovementDirectionMultiplier;

	updateTransformationMatrix();
	wheelFront->updateTransformationMatrix();
	wheelBack->updateTransformationMatrix();
	bodyTransparent->updateTransformationMatrix();
	updateCollisionModel();
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

void MH_YellowMovingPlatform::setupModelWheelFront()
{
	wheelFront = new Body(&MH_YellowMovingPlatform::modelsWheelFront);
	wheelFront->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntityPass2(wheelFront);
	wheelFront->setPosition(&position);
	wheelFront->setRotY(rotY);
	
	//move wheel to correct position based on axis
	if (platformMovesOnXAxis == true)
	{
		wheelFront->position.z += wheelOffsetFrontHorizontal * wheelMovementDirectionMultiplier;
	}
	else
	{
		wheelFront->position.x += wheelOffsetFrontHorizontal * wheelMovementDirectionMultiplier;
	}
	wheelFront->position.y += wheelOffsetFrontVertical;
}

void MH_YellowMovingPlatform::setupModelWheelBack()
{
	wheelBack = new Body(&MH_YellowMovingPlatform::modelsWheelBack);
	wheelBack->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntityPass2(wheelBack);
	wheelBack->setPosition(&position);
	wheelBack->setRotY(rotY);
	
	//move wheel to correct position based on axis
	if (platformMovesOnXAxis == true)
	{
		wheelBack->position.z += wheelOffsetBackHorizontal * wheelMovementDirectionMultiplier;
	}
	else
	{
		wheelBack->position.x += wheelOffsetBackHorizontal * wheelMovementDirectionMultiplier;
	}
	
	wheelBack->position.y += wheelOffsetBackVertical;
}

void MH_YellowMovingPlatform::setupModelTransparent()
{
	bodyTransparent = new Body(&MH_YellowMovingPlatform::modelsTransparent);
	bodyTransparent->setVisible(true);
	INCR_NEW("Entity");
	Main_addEntityPass3(bodyTransparent);
	bodyTransparent->setPosition(&position);
	bodyTransparent->setRotY(rotY);
}