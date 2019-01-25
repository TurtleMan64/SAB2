#include <cmath>

#include "body.h"
#include "keyframe.h"
#include "../toolbox/maths.h"
#include "limb.h"



Limb::Limb(std::list<TexturedModel*>* models, float pivotX, float pivotY, float pivotZ, Body* body, Limb* limb)
{
	myModels = models;
	this->pivotX = pivotX;
	this->pivotY = pivotY;
	this->pivotZ = pivotZ;
	this->body = body;
	this->limb = limb;
	setVisible(true);
}

void Limb::step()
{

}

void Limb::update(float time)
{
	time = (float)fmax(time, 0);
	bool inRange = false;
	Keyframe* key1 = nullptr;
	Keyframe* key2 = nullptr;


	float angleX, angleY, angleZ, angleS;
	if (body != nullptr)
	{
		angleX = Maths::toRadians(body->getRotX());
		angleY = Maths::toRadians(body->getRotY());
		angleZ = Maths::toRadians(body->getRotZ());
		angleS = Maths::toRadians(body->getRotSpin());
	}
	else
	{
		angleX = Maths::toRadians(limb->getRotX());
		angleY = Maths::toRadians(limb->getRotY());
		angleZ = Maths::toRadians(limb->getRotZ());
		angleS = Maths::toRadians(limb->getRotSpin());
	}


	float newX = pivotX, newY = pivotY, newZ = pivotZ;

	float newPivotX = newX;
	float newPivotY = newY;
	float newPivotZ = newZ;

	//rotation around z axis
	newX =  (newPivotX * cosf(angleS) - newPivotY * sinf(angleS));
	newY = -(newPivotY * cosf(angleS) + newPivotX * sinf(angleS));
	newPivotX = newX;
	newPivotY = newY;
	newPivotZ = newZ;


	//rotation around x axis: this one might not be right
	newZ =  (newPivotZ * cosf(angleX) - newPivotY * sinf(angleX));
	newY = -(newPivotY * cosf(angleX) + newPivotZ * sinf(angleX));
	newPivotX = newX;
	newPivotY = newY;
	newPivotZ = newZ;


	//rotation around z axis
	newX = (newPivotX * cosf(angleZ) - newPivotY * sinf(angleZ));
	newY = (newPivotX * sinf(angleZ) + newPivotY * cosf(angleZ));
	newPivotX = newX;
	newPivotY = newY;
	newPivotZ = newZ;


	//rotation around y axis
	newX = (newPivotX * cosf(angleY) + newPivotZ * sinf(angleY));
	newZ = (newPivotZ * cosf(angleY) - newPivotX * sinf(angleY));


	float newXRot = 0;
	float newYRot = 0;
	float newZRot = 0;
	float newSRot = 0;
	float newScale = 0;

	for (unsigned int i = 0; i < (*animations)[animationIndex].keyframes.size() - 1; i++)
	{
		key1 = &(*animations)[animationIndex].keyframes[i];
		key2 = &(*animations)[animationIndex].keyframes[i + 1];

		if (time >= key1->time && time <= key2->time)
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
		float t = (time - key1->time) / (key2->time - key1->time);

		//sinusoidal interpolation
		t = 1 - ((float)(cos(Maths::PI*t) + 1)*0.5f);

		float ratio = t;

		newXRot = key1->xRot + ratio*(key2->xRot - key1->xRot);
		newYRot = key1->yRot + ratio*(key2->yRot - key1->yRot);
		newZRot = key1->zRot + ratio*(key2->zRot - key1->zRot);
		newSRot = key1->sRot + ratio*(key2->sRot - key1->sRot);
		newScale = key1->scale + ratio*(key2->scale - key1->scale);
	}
	else
	{
		newXRot = key2->xRot;
		newYRot = key2->yRot;
		newZRot = key2->zRot;
		newSRot = key2->sRot;
		newScale = key2->scale;
	}

	if (body != nullptr)
	{
		position.x = (body->getX() + newX);
		position.y = (body->getY() + newY);
		position.z = (body->getZ() + newZ);
		setRotX(body->getRotX() + newXRot); //twist
		setRotY(body->getRotY() + newYRot);
		setRotZ(body->getRotZ() + newZRot);
		setRotSpin(body->getRotSpin() + newSRot); //spin
		//super.setScale(newScale);
	}
	else
	{
		position.x = (limb->getX() + newX);
		position.y = (limb->getY() + newY);
		position.z = (limb->getZ() + newZ);
		setRotX(limb->getRotX() + newXRot); //twist
		setRotY(limb->getRotY() + newYRot);
		setRotZ(limb->getRotZ() + newZRot);
		setRotSpin(limb->getRotSpin() + newSRot); //spin
		setScale(getScale()*newScale);
	}
}

std::list<TexturedModel*>* Limb::getModels()
{
	return myModels;
}
