#ifndef ROCKET_H
#define ROCKET_H

class TexturedModel;
class Body;
class Source;

#include <list>
#include "./entity.h"
#include "./collideableobject.h"


class Rocket : public CollideableObject
{
private:
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

	//What fraction of the path has been completed, rather than actually moving the Rocket, it instead just
	//teleports to the correct spot in the path, which simulates movement
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
	//The player is within the range for the rocket appear sound to play, and the sound hasn't been played/has been reset
	bool rocketAppearSoundCanPlay();
	//The player is outside the range for the rocket appear sound to set so it plays again when you get close.
	//This is farther out than the spot where the appear sound plays so you can't run in a circle to spam it
	bool rocketAppearSoundCanReset();
	//The player is inside the rockets hitbox
	bool playerWithinRocketHitbox();
	//Make the dirt particles used to make the rocket's exhaust
	void makeDirtParticles(float particlePositionOffset);
	//Plays the portion of the sound of the rocket launching before the loop begins
	void playRocketLaunchSoundStart();
	//Plays the looping portion of the sound of the rocket launching
	void playRocketLaunchSoundLoop();
	//If the rocket has started moving, the startup animation has finished
	bool rocketStartedMoving();
	//Calculate the player's position so they hold the rocket handle
	Vector3f calculateNewPlayerPosition();
	//Calculates the new position of the rocket for the new frame
	Vector3f calculateNewRocketPosition();
	//Calculates the new value of percentOfPathCompleted, which is used to teleport the rocket and player to that position on the path
	//This simulates movement, since the updates happen every single frame
	float calculatePathMovementPercentForThisFrame();
	//Returns true if the percentOfPathCompleted is 100% (equal to 1)
	bool fullPathTraveled();
	//Resets the rocket's position, canActivate, isActive, percentOfPathCompleted, and startupTimer to default values
	void resetRocketVariables();

public:
	Rocket();
	Rocket(int point1ID, int point2ID);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif