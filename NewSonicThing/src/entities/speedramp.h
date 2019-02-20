#ifndef SPEEDRAMP_H
#define SPEEDRAMP_H

class TexturedModel;

#include <list>
#include "entity.h"
#include "collideableobject.h"

class SpeedRamp : public CollideableObject
{
private:
	//visual model of the speedramp
	static std::list<TexturedModel*> models;
	//collision model of the speedramp
	static CollisionModel* cmOriginal;

	//power of the speedramp, how far it sends the player
	float speedRampPower;
	//how long the player can't move after using the speedramp
	float inputLockDuration;

	//functions for the contructor start here

	//set the collision model up, not much more to say
	void setupCollisionModel();

	//functions for step() start here

	//calculate the new velocity of the player when they step on the speedramp
	Vector3f calculateNewVelocity();
	//play the sound the speed ramp makes when you step on it
	void playSpeedRampSound();

public:
	SpeedRamp();
	SpeedRamp(float x, float y, float z, float rotY, float rotZ, float myPower, float inputLockDuration);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif