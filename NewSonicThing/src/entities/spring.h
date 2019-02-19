#ifndef SPRING_H
#define SPRING_H

class TexturedModel;

#include <list>
#include "entity.h"


class Spring : public Entity
{
private:
	static std::list<TexturedModel*> models;

	float springPower;
	float springRadius;
	int cooldownTimer;
	int cooldownTimerMax;

	Vector3f directionOfMovement;

	//functions for step() start here

	//Use the position of the player and the position of the spring to calculate if the player is in rangeS
	bool playerIsInRange();
	//Calculate the vector for the direction of movement using the z and y rotation
	Vector3f calculateDirectionOfMovement();
	//Set the players position to slightly above the Spring
	void setPlayerPositionToAboveSpring();
	//Set the players velocity using the springs direction and power
	void setPlayerVelocityToSpringAway();
	//Play the noise the spring makes when you bounce off it
	void playSpringSound();

public:
	Spring();
	Spring(float x, float y, float z, float rotY, float rotZ, float myPower, float cooldownMax);

	void step();

	float getSpringPower();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();

	std::string getName();

	bool canHomingAttackOn();

	Vector3f getHomingCenter();
};
#endif