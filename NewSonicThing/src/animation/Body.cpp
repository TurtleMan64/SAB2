#include <cmath>

#include "body.h"
#include "keyframe.h"
#include "../toolbox/maths.h"



Body::Body(std::list<TexturedModel*>* models)
{
	myModels = models;
	baseX = 0;
	baseY = 0;
	baseZ = 0;
	baseRotY = 0;
	baseRotZ = 0;
	baseRotX = 0;
	baseRotS = 0;
	animationIndex = 0;
	time = 0;
	prevTime = 0;
	deltaTime = 0;
	setVisible(true);
}

void Body::step()
{

}

void Body::update(float newTime)
{
	prevTime = time;
	time = fmaxf(newTime, 0);
	newTime = time;
	deltaTime = time - prevTime;
	bool inRange = false;
	Keyframe* key1 = nullptr;
	Keyframe* key2 = nullptr;
	float newX = 0;
	float newY = 0;
	float newZ = 0;
	float newXRot = 0;
	float newYRot = 0;
	float newZRot = 0;
	float newSRot = 0;
	//float newScale = 0;

	for (unsigned int i = 0; i < (*animations)[animationIndex].keyframes.size() - 1; i++)
	{
		key1 = &(*animations)[animationIndex].keyframes[i];
		key2 = &(*animations)[animationIndex].keyframes[i + 1];

		if (newTime >= key1->time && newTime <= key2->time)
		{
			inRange = true;
			break;
		}
	}

	if (inRange)
	{
		//linear interpolation
		//view key1 as the "base"
		//and add a certain ratio of the difference between
		//key2 and key1 to key1 for the interpolated value
		//float ratio = (time-key1.time)/(key2.time-key1.time);
		float t = (newTime - key1->time) / (key2->time - key1->time);

		//sinusoidal interpolation
		t = 1 - (((float)cos(M_PI*t) + 1)*0.5f);

		float ratio = t;

		newX = key1->x + ratio*(key2->x - key1->x);
		newY = key1->y + ratio*(key2->y - key1->y);
		newZ = key1->z + ratio*(key2->z - key1->z);
		newXRot = key1->xRot + ratio*(key2->xRot - key1->xRot);
		newYRot = key1->yRot + ratio*(key2->yRot - key1->yRot);
		newZRot = key1->zRot + ratio*(key2->zRot - key1->zRot);
		newSRot = key1->sRot + ratio*(key2->sRot - key1->sRot);
		//newScale = key1->scale + ratio*(key2->scale - key1->scale);
	}
	else
	{
		newX = key2->x;
		newY = key2->y;
		newZ = key2->z;
		newXRot = key2->xRot;
		newYRot = key2->yRot;
		newZRot = key2->zRot;
		newSRot = key2->sRot;
		//newScale = key2->scale;
	}
	
	float newXOffset = newX;
	float newYOffset = newY;
	float newZOffset = newZ;

	float angleX = Maths::toRadians(baseRotX + newXRot);
	float angleY = Maths::toRadians(baseRotY + newYRot);
	float angleZ = Maths::toRadians(baseRotZ + newZRot);
	float angleS = Maths::toRadians(baseRotS + newSRot);


	//rotation around z axis
	newX =  (newXOffset * cosf(angleS) - newYOffset * sinf(angleS));
	newY = -(newYOffset * cosf(angleS) + newXOffset * sinf(angleS));
	newXOffset = newX;
	newYOffset = newY;
	newZOffset = newZ;


	//rotation around x axis: this one might not be right
	newZ =  (newZOffset * cosf(angleX) - newYOffset * sinf(angleX));
	newY = -(newYOffset * cosf(angleX) + newZOffset * sinf(angleX));
	newXOffset = newX;
	newYOffset = newY;
	newZOffset = newZ;


	//rotation around z axis
	newX = (newXOffset * cosf(angleZ) - newYOffset * sinf(angleZ));
	newY = (newXOffset * sinf(angleZ) + newYOffset * cosf(angleZ));
	newXOffset = newX;
	newYOffset = newY;
	newZOffset = newZ;


	//rotation around y axis
	newX = (newXOffset * cosf(angleY) + newZOffset * sinf(angleY));
	newZ = (newZOffset * cosf(angleY) - newXOffset * sinf(angleY));


	position.x = (baseX + newX);
	position.y = (baseY + newY);
	position.z = (baseZ + newZ);
	setRotX(baseRotX + newXRot);
	setRotY(baseRotY + newYRot);
	setRotZ(baseRotZ + newZRot);
	setRotSpin(baseRotS + newSRot);
	//setScale(newScale);
}

void Body::setBaseOrientation(Vector3f* basePosition, float newBRotX, float newBRotY, float newBRotZ, float newBRotS)
{
	baseX = basePosition->x;
	baseY = basePosition->y;
	baseZ = basePosition->z;
	baseRotX = newBRotX;
	baseRotY = newBRotY;
	baseRotZ = newBRotZ;
	baseRotS = newBRotS;
}

void Body::setBaseOrientation(float newBaseX, float newBaseY, float newBaseZ, float newRotX, float newRotY, float newRotZ, float newRotS)
{
	baseX = newBaseX;
	baseY = newBaseY;
	baseZ = newBaseZ;
	baseRotX = newRotX;
	baseRotY = newRotY;
	baseRotZ = newRotZ;
	baseRotS = newRotS;
}

//void Body::setBaseRotZ(float rotZ)
//{
	//baseRotZ = rotZ;
//}

std::list<TexturedModel*>* Body::getModels()
{
	return myModels;
}

void Body::setModels(std::list<TexturedModel*>* newModels)
{
	myModels = newModels;
}