#ifndef MH_YELLOWMOVINGPLATFORM
#define MH_YELLOWMOVINGPLATFORM

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;
class Body;

class MH_YellowMovingPlatform : public CollideableObject
{
private:
	enum states
	{
		MOVING_FORWARD,
		MOVING_BACKWARDS,
		STOPPED
	};
	int stateCurrent;

	//The visual model that doesn't use vertex colours
	static std::list<TexturedModel*> models;
	//The visual model for the front wheels
	static std::list<TexturedModel*> modelsWheelFront;
	//The visual model for the back wheels
	static std::list<TexturedModel*> modelsWheelBack;
	//The visual model that is for textures with transparency
	static std::list<TexturedModel*> modelsTransparent;
	//The collision model
	static CollisionModel* cmOriginal;

	//Horizontal Offset of the Front Wheel
	const float WHEEL_OFFSET_FRONT_HORIZONTAL = 8.86459;
	//Horizontal Offset of the Back Wheel
	const float WHEEL_OFFSET_BACK_HORIZONTAL = -13.1062;
	//Vertical Offset of the Front Wheel
	const float WHEEL_OFFSET_FRONT_VERTICAL = 4.28373;
	//Vertical Offset of the Back Wheel
	const float WHEEL_OFFSET_BACK_VERTICAL = 5.71319;
	//Speed of the Front Wheel
	const float WHEEL_SPEED_FRONT = 2;
	//Speed of the Back Wheel, different because it's a bigger wheel
	const float WHEEL_SPEED_BACK = 1.885;
	//minimum value of the shake timer
	const float SHAKE_TIMER_MIN = 3.14159265359;
	//maximum value of the shake timer
	const float SHAKE_TIMER_MAX = 31.4159265359;

	//Body used to be able to display the front wheels model
	Body* wheelFront;
	//Body used to be able to display the back wheels model
	Body* wheelBack;
	//Body used to be able to display the transparent model
	Body* bodyTransparent;
    
    //initial position of the platform
    Vector3f positionInitial;
    //controls whether the platform moves in the x axis or z axis
    bool platformMovesOnXAxis;
	//determines the directions the wheels are rotated
	//in step, changing the direction is a multiplation with -1, so to flip between forwards or backwards at the start, set to -1
	int wheelMovementDirectionMultiplier;
    //the max displacement from the starting position
    float displacementMax;
	//current displacement, used for keeping track of how far the pulley has moved, so you can stop it at the destination
	float displacementCurrent;
    //speed at which the platform moves
    float speed;
	//the direction the platform moves in, used for making movement possible, value calculated
	//in calculateDirectionVector(), called from the constructor
	Vector3f directionVector;
	//keeps track of how long the platform has been stopped for, so it can start moving again
	float stoppedTimer;
	//position the platform stopped at, used for the shaking
	Vector3f positionStopped;
	//used for the shaking of the platform, dt * 20 is added to it to speed up the shaking animation
	float shakeTimer;

	//used for player collisions with the front and back of the platform when moving
	Vector3f collisionPositionFront1;
	Vector3f collisionPositionFront2;
	Vector3f collisionPositionBack1;
	Vector3f collisionPositionBack2;
	const float COLLISION_POSITION_SIDEWAYS = 5;
	const float COLLISION_POSITION_FORWARD = 12;
	const float COLLISION_RADIUS = 10; 
	const float COLLISION_HEIGHT = 11.5;


	//Functions for the constructor start here
	
	//calculates the value of directionVector
	Vector3f calculateDirectionVector();
	//Makes the body used for the front wheels model
	void setupModelWheelFront();
	//Makes the body used for the back wheels model
	void setupModelWheelBack();
	//Makes the body used for the transparent model
	void setupModelTransparent();

	//Functions for step() start here
	
	//Moves the platform towards the point specified in the arguments
	Vector3f calculateMovementAmount(Vector3f displacementVectorLocal);
	//Ensures the wheels and other separate pieces have their correct positions
	//Relative to last position
	void syncBodyPositionsRelative(Vector3f movementAmount);
	//Ensures the wheels and other separate pieces have their correct positions
	//Based on the position of the platform, desync impossible, but more code
	void syncBodyPositionsAbsolute();
	//Makes the wheels spin, based on the direction specified by wheelMovementDirectionMultiplier
	void spinWheels();
	//Moves the player to keep them in sync with the moving platform
	void movePlayer(Vector3f movementAmount);
	//Checks if the player is touching the front of the platform, and if so, pushes them away
	//prevents clipping into the platform. 
	//Uses collisionPositionFront1 and collisionPositionFront2 as positions to generate the circles, 
	//and uses collisionRadius and collisionHeight for the other dimensions of the circles
	void playerCollisionCheckFront();
	//Checks if the player if touching the front of the platform, and if so, pushes them away
	//prevents clipping into the platform. 
	//Uses collisionPositionBack1 and collisionPositionBack2 as positions to generate the circles, 
	//and uses collisionRadius and collisionHeight for the other dimensions of the circles
	void playerCollisionCheckBack();
	//Creates the shaking effect of the platform when it stops
	Vector3f shakePlatform();

public:
	MH_YellowMovingPlatform(float x, float y, float z, int platformMovesOnXAxis, float displacementMax, float speed);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif