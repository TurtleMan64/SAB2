#ifndef PULLEY
#define PULLEY

#include <list>
#include "entity.h"
#include "collideableobject.h"

class TexturedModel;
class Body;
class Source;

class Pulley : public CollideableObject
{
private:
	//Visual model of the handle.
	static std::list<TexturedModel*> modelsHandle;
	//Visual model of the rope.
	static std::list<TexturedModel*> modelsRope;
	//Visual model of the top of the pulley.	
	static std::list<TexturedModel*> modelsTop;
	//Collision model of the top of the pulley.
	static CollisionModel* cmTop;

	//The entity of the rope.
	Body* rope;
	//The entity of the top of the rocket.
	Body* top;

	//How far down the handle is from the top of the rocket.
	//Used for both the position of the handle and the length of the rope.
	//This gets moved dependi
    float handleVerticalDisplacement;
	//The very bottom point of how far
	float handleVerticalDisplacementBottom;
	//The closest the handle can get to the top of the pulley
	const float HANDLE_VERTICAL_DISPLACEMENT_MINIMUM = 10;

	//Radius of the horizontal component of the hitbox of the handle
	const float HITBOX_RADIUS = 5;
	//Height of the hitbox of the handle
	const float HITBOX_HEIGHT = 5;

	//Player is riding the pulley
	bool playerIsOnPulley = false;

	//functions for the constructor start here
	
	//Setup the visual model of the rope of the pulley as a separate entity, 
	//allowing it to move separately from the handle.
	void setupPulleyRope();
	//Setup the visual model and collision of the top of the pulley as a separate entity, 
	//allowing it to move separately from the handle.
	void setupPulleyTop();

	//functions for step() start here

	//Check if the player has collided with the rocket handle.
	bool playerWithinHandleHitbox();
	//Check if the pulley is at the bottom position where it can be grabbed.
	bool handleAtBottom();
	//Check if the pulley is at the very top where it stops moving.
	bool handleAtTop();
	//Play and loop the sound the pulley makes while it moves.
	void playPulleySound();
	//Move the pulley either up or down, depending on if true or false
	void movePulley(bool movePulleyUp);
	//Check if the player has jumped.
	bool jumpInputPressed();

public:
	Pulley();
    Pulley(float x, float y, float z, float rotY, float handleVerticalDisplacement);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif