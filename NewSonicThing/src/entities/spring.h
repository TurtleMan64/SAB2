#ifndef SPRING_H
#define SPRING_H

class TexturedModel;

#include <list>
#include "entity.h"


class Spring : public Entity
{
private:
	//Springs visual model
	static std::list<TexturedModel*> models;

	//power of the spring, how far it sends the player
	float springPower;
	//radius of the spring, how far away from is is still considered touching
	const float SPRING_RADIUS = 7.8f;
	//Current value of the cooldown timer, when this is 0 you can interact with the spring,
	//otherwise dt is subtracted from it until it is 0
	float cooldownTimer;
	//What the cooldown timer starts counting from when reset, also what the player canMove timer is set to
	float cooldownTimerInitialValue;
	//The direction of the movement the spring will send you in, calculated using the y and z rotation of the spring
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

	const bool canHomingAttackOn();

	const Vector3f getHomingCenter();
};
#endif