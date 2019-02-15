#ifndef ROCKETGENERAL_H
#define ROCKETGENERAL_H

class TexturedModel;
class Body;
class Source;

#include <list>
#include "./entity.h"
#include "./collideableobject.h"


class RocketGeneral : public CollideableObject
{
protected:
	void rocketConstructor(int point1ID, int point2ID);

	void rocketStep();

	//Visual model of the rocket
	static std::list<TexturedModel*> modelsRocket;
	//Visual model of the base
	static std::list<TexturedModel*> modelsBase;
	//Collision model of the base
	static CollisionModel* cmBase;

	//Used to make making the base a separate entity possible
	Body* base;

	//Position of the start point of the path
	Vector3f pointPositionStart;
	//Position of the end point of the path
	Vector3f pointPositionEnd;

	//The speed the rocket moves at
	const float ROCKET_SPEED = 900;
	//True only when the player is using the Rocket
	bool isActive;
	//Whether the Rocket can be used or not
	bool canActivate;

	//Radius of the Rocket's hitbox
	const float HITBOX_RADIUS = 5;
	//Height of the Rocket's hitbox
	const float HITBOX_HEIGHT = 15;

	//Difference of the position of the rocket at the start and the end of the path
	Vector3f rocketPathPositionDifference;
	//Normalized version of the difference of the position of the rocket at the start and the end of the path
	Vector3f rocketPathPositionDifferenceNormalized;
	//Length of the vector between the rocket at the start and the end of the path
	float rocketPathPositionDifferenceLength;

	//The difference in position from the player and the rocket.
    Vector3f playerToRocketPositionDifference;
	//Squared horizontal difference between the players position and the rockets position
	float playerToRocketPositionDifferenceHorizontalSquared;

	//Uses the speed, length of the path, and delta time to calculate how much of the path is completed every frame
	float percentOfPathCompleted;
	//Timer used for the startup animation of the rocket, where it waits before shooting off
	float startupTimer;
	//The value the startupTimer counts down from
	const float STARTUP_TIMER_INITIAL_VALUE = 0.5;

	//particle position offset from the rockets position when the rocket is starting up
	const float PARTICLE_POSITION_OFFSET_ROCKET_STARTUP = -2;
	//particle position offset from the rockets position when the rocket is moving
	const float PARTICLE_POSITION_OFFSET_ROCKET_MOVING = 0;

	//Whether the sound that plays when you get close to the Rocket has played
	bool rocketAppearSoundPlayed;
	//The audio source used to play the Rocket's audio
	Source* rocketAudioSource;

	//Functions for the constructor start here

	//Returns the position of the point with the specified ID
	Vector3f getPointPosition(int pointID);
	//Sets the rocket base's collision up and also adds it as an entity since it has to move separately from the Rocket
	void setupRocketBase();
	//Calculate the Y rotation value of the Rocket so it horizontally faces the end path
	float calculateRocketYRotation();
	//Calculate the Z rotation value so it vertically points towards the end of the path
	float calculateRocketZRotation();

	//Functions for step() start here

	//Gets the squared horizontal difference between the players position and the rockets position
	float getPlayerToRocketDifferenceHorizontalSquared();
	//The player is within the range for the rocket appear sound to play
	bool playerWithinAppearSoundRange();
	//The player is outside the range for the rocket appear sound to set so it plays again when you get close
	//This is farther out than the spot where the appear sound plays so you can't run in a circle to spam it
	bool playerOutsideAppearSoundResetRange();
	//The player is inside the rockets hitbox
	bool playerWithinRocketHitbox();
	//Make the dirt particles used to make the rocket's exhaust
	void makeDirtParticles(float particlePositionOffset);
	//Plays the sound of the rocket launching
	void playRocketLaunchSound();
	//Set the players velocity, onRocket, isBall, RotY, CanMoveTimer, and OnGround
	void setPlayerVariablesRocketActive();
	//If the rocket has started moving, the startup animation has finished
	bool rocketStartedMoving();
	//Set the player's position so they hold the rocket handle
	void setPlayerPositionToHoldRocketHandle();
	//Calculates the new position of the rocket for the new frame
	void calculateNewRocketPosition();
	//Calculates the new value of percentOfPathCompleted using the speed, length of the path, and dt
	void calculateNewPercentOfPathCompletedValue();
	//Returns true if the percentOfPathCompleted is 100% (equal to 1)
	bool fullPathTraveled();
	//Sets player velocity, onRocket, and canMoveTimer for the end of the rocket's path
	void setPlayerVariablesRocketStopping();
	//Resets the rocket's position, canActivate, isActive, percentOfPathCompleted, and startupTimer to default values
	void resetRocketVariables();

public:
	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif