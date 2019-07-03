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

	//Body used to be able to display the front wheels model
	Body* wheelFront;
	//Body used to be able to display the back wheels model
	Body* wheelBack;
	//Body used to be able to display the transparent model
	Body* bodyTransparent;
    
    //initial position of the platform
    Vector3f positionInitial;
    //controls whether the platform moves in the x axis (axis=0) or z axis (axis=1)
    int axis;
    //the max displacement from the starting position, change direction w/ axis and postive/negative
    //example: axis=1 and displacement=-5 means move -5 units from starting position on the z axis
    float displacementMax;
    //speed at which the platform moves
    float speed;

	//Horizontal Offset of the Front Wheel
	const float wheelOffsetFrontHorizontal = 8.86459;
	//Horizontal Offset of the Back Wheel
	const float wheelOffsetBackHorizontal = -13.1062;
	//Vertical Offset of the Front Wheel
	const float wheelOffsetFrontVertical = 4.28373;
	//Vertical Offset of the Back Wheel
	const float wheelOffsetBackVertical = 5.71319;
	//Speed of the Front Wheel
	const float wheelSpeedFront = 2;
	//Speed of the Back Wheel
	const float wheelSpeedBack = 1.885;

	//Functions for the constructor start here
	
	//Makes the body used for the front wheels model
	void setupModelWheelFront();
	//Makes the body used for the back wheels model
	void setupModelWheelBack();
	//Makes the body used for the transparent model
	void setupModelTransparent();

public:
	MH_YellowMovingPlatform(float x, float y, float z, float rotY, int axis, float displacementMax, float speed);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif