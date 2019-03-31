#ifndef CAR_H
#define CAR_H

class TexturedModel;
class Triangle3D;
class ParticleTexture;
class Source;
class ManiaSonicModel;
class Body;
class Limb;

#include <list>
#include "entity.h"
#include "../toolbox/vector.h"

class Car : public Entity
{
private:
	static std::list<TexturedModel*> modelBody;
	static std::list<TexturedModel*> modelHead;
	static std::list<TexturedModel*> modelLeftHumerus;
	static std::list<TexturedModel*> modelLeftForearm;
	static std::list<TexturedModel*> modelLeftHand;
	static std::list<TexturedModel*> modelLeftThigh;
	static std::list<TexturedModel*> modelLeftShin;
	static std::list<TexturedModel*> modelLeftFoot;
	static std::list<TexturedModel*> modelRightHumerus;
	static std::list<TexturedModel*> modelRightForearm;
	static std::list<TexturedModel*> modelRightHand;
	static std::list<TexturedModel*> modelRightThigh;
	static std::list<TexturedModel*> modelRightShin;
	static std::list<TexturedModel*> modelRightFoot;

	Body* myBody;
	Limb* myHead;
	Limb* myLeftHumerus;
	Limb* myLeftForearm;
	Limb* myLeftHand;
	Limb* myLeftThigh;
	Limb* myLeftShin;
	Limb* myLeftFoot;
	Limb* myRightHumerus;
	Limb* myRightForearm;
	Limb* myRightHand;
	Limb* myRightThigh;
	Limb* myRightShin;
	Limb* myRightFoot;

	ManiaSonicModel* maniaSonicModel;

	bool onGround = false;
	Vector3f vel; //Direction we are currently going
	Vector3f relativeUp; //What direction is "up", aka grounds normal
	Vector3f relativeUpSmooth; //What direction is "up", aka grounds normal
	Vector3f relativeUpAnim; //relative up to use for animations. smooth less than relativeUpSmooth
	Vector3f camDir; //Direction the camera wants to be facing
	Vector3f camDirSmooth; //Direction the camera is facing curerntly

	const float FLOOR_OFFSET = 0.1f; //How much you stay away from the floor
	Triangle3D* currentTriangle = nullptr; //the triangle object that you are touching
	const float smoothTransitionThreshold = 0.6f; //Dot product threshold for running between triangles
	const float surfaceTension = 10.0f;     //To not fly off the ground
	const float hitWallTimePunish = 0.0f;   //How long you can't move after hitting a wall 0.125
	const float wallStickThreshold = 0.45f;  //How steep a slope must be to be considered a wall 0.3

	const float gravityForce = 280.0f;
	const float gravityTerminal = -550.0f;
	const float gravityApproach = 0.4f;

	bool isJumping = false;
	const float jumpPower = 102.0f; //Initial speed you get form jump
	const float hoverPower = 144.0f; //How much hover adds to your vel
	const float hoverTimerThreshold = 1.0f; //How long you can hover for
	float hoverTimer = 0.0f;

	const float groundRunPush = 100.0f;
	const float groundRunFriction = 0.5f;
	const float groundNeutralFriction = 5.5f;
	const float skidPower = -5.0f;
	const float airRunPush = 25.0f; //30.0
	const float airRunFrictionUp = 0.75f; //0.25    //maybe make another variable for this to use when vel.y < 0?
	const float airRunFrictionDown = 0.25f; //For when sonic is falling down
	const float airNeutralFriction = 1.25f;
	const float slopeRunAccel = 9000.0f;

	bool isBall = false;
	//version 1
	//const float groundBallFriction = 0.1f;
	//const float slopeBallAccel = 42000.0f;
	//version 2
	const float groundBallFriction = 0.1f; //0.02f;
	const float slopeBallAccel = 140.0f; //50.0f;
	const float autoUnrollThreshold = 120.0f; //60
	const float maxBallSpeed = 800.0f; //you wont go faster than this when rolling down a hill in ball form

	bool isSpindashing = false;
	Vector3f spindashDirection;
	bool canStartSpindash = false;
	bool bufferedSpindashInput = false;
	const float spindashPowerMax = 525.0f; //475.0f
	const float spindashChargeRate = 0.4f*60*60;
	const float spindashFriction = 2.0f;
	const float spindashPowerfulFriction = 8.0f;
	const float spindashPowerfulFrictionThreshold = 100.0f;
	const float spindashDelay = 0;
	float spindashTimer = 0;
	const float spindashTimerMax = 35.0f/60.0f;
	float spindashReleaseTimer = 0;
	const float spindashReleaseTimerMax = 30.0f/60.0f;
	float spindashRestartDelay = 0;
	const float spindashRestartDelayMax = 25.0f/60.0f; // 3.0f/60.0f
	float storedSpindashSpeed = 0;

	bool isSkidding = false;
	bool isStomping = false;
	bool isLightdashing = false;
	bool isGrinding = false;
    bool onRocket = false;

	const float bounceVel = -300.0f;
	const float bounceFactor = 0.7f;
	bool isBouncing = false;
	bool justBounced = false; //Are we in the air after doing a bounce?

	bool justHomingAttacked = false; //Are we in the air after doing a homing attack?
	float homingAttackTimer = -1.0f; // less than 0 = can start a homing attack
	const float homingAttackTimerMax = 0.1666f;

	//const float camAngleLookdown = 0.0f; //0.2f; //How much the camera looks down
	const float camRadius        = 75.0;  //Camera radius at zero speed
	const float camHeightOffset  = 5.0f; //Point above the vehicle that the camera focuses on

	//const float 
	

	//const float camAngleLookdown = 0.2f;    //How much the camera looks down
	//const float camRadius        = 7.5;     //Camera radius at zero speed
	//const float camRadiusAdjust  = 0.0005f; //How much the camera zooms out due to speed
	//const float camFovAdjust     = 0.05f;   
	//const float camHeightOffset  = 2.5f;    //Point above the vehicle that the camera focuses on
	//const float camSmoothness    = 5.0f;    //Smoothes currNormSmooth and camDirSmooth
	//
	//float terminalSpeed      = 298.0f; //Normal speed cap
	//float terminalAccelGas   = 0.45f;  //How fast you accelerate towards terminal speed
	//float terminalAccelBrake = 1.0f;   //How fast you accelerate towards 0 while braking
	//float terminalAccelCoast = 0.1f;   //How fast you accelerate towards 0 while coasting
	//
	//float turnSpeed  = 1.38f; //How fast you turn
	//float turnPunish = 0.01f; //How much you slow down due to turning
	//
	//float diveSpeed  = 4.5f; //How fast you dive both up and down 
	//float divePunish = 2.5f; //How much you are punished for pulling up
	//
	//float strafePercentage     = 0.2f;  //Percent that strafing goes sideways
	//float strafeTerminalPunish = 0.95f; //Percent that strafing lowers the terminal speed
	//
	//float slipTimer          = 0.0f;
	//float slipAccumulated    = 0.0f;  //How much slip you have accumulated from jerking the wheel
	//float slipTimerMax       = 0.75f; //What the slipTimer gets set to when you trigger a slip
	//float slipThreshold      = 4.5f;  //Point at which you trigger a slip
	//float slipAngle          = 0.0f;  //Current angle that you are facing during the slip
	//float slipAngleTarget    = 0.0f;  //What the slip angle is approaching
	//float slipAngleAccel     = 5.0f;  //How fast the slip angle approaches the target
	//float slipAngleMax       = 40.0f; //Max that the angle can slip by
	//float slipPower          = 1.5f;  //How much you turn when you slip turn
	//float slipNegativePower  = 1.1f;  //Multiplier for when you lose speed during slipping
	//float slipPositivePower  = 0.75f; //Multiplier for when you gain speed during slipping
	//float slipTimerRight     = 0.0f;  //How long you have been slipping to the right for
	//float slipTimerLeft      = 0.0f;  //How long you have been slipping to the left for
	//float slipTimerThreshold = 0.5f;  //Time threshold for when you start loosing speed vs gaining it during slipping
	//
	//float spinTimeMax   = 1.65f; //How long the spin attack lasts
	//float spinTimeDelay = 2.5f;  //How long until you can do another spin attack
	//float spinTimer     = 0.0f;
	//
	//float sideAttackTimer   = 0.0f;
	//float sideAttackDir     = 1.0f;    //Direction of the side attack
	//float sideAttackTimeMax = 0.1666f; //How long you side attack for
	//float sideAttackSpeed   = 150.0f;  //How fast you move during the attack at the beginning
	//
	//float boostSpeed        = 462.0f; //Max speed when boosting
	//float boostKick         = 0.7f;   //Initial speed addition you get from starting a boost
	//float boostDuration     = 1.0f;   //How long you boost for
	//float boostDelayMax     = 1.15f;  //Time until you can boost again
	//float boostDelayTimer   = 0.0f;
	//float boostHealthPunish = 0.2f;   //How much health you loose for a boost
	//
	//      float camAngleAdditionalLookdown                 = 0.0f; //Additional lookdown due to driving through a steep concave or convex area
	//const float camAngleAdditionalLookdownSmoothness       = 0.4f; //How fast the lookdown approaches the target
	//      float camAngleAdditionalLookdownTarget           = 0.0f;
	//const float camAngleAdditionalLookdownTargetSmoothness = 2.5f; //How fast the target approaches 0
	//const float camAngleAdditionalLookdownMax              = 0.3f; //Max value of the lookdown
	//const float camAngleAdditionalLookdownScale            = 0.005f; //How much the target gets added to by each new triangle collision
	//
	//float health   = 1.0f;
	//float healRate = 0.666f; //Health per second that you recover while in health pads
	//float hitWallHealthPunish = 0.3f; //How much health you loose when hitting a wall perpendicularly at max speed
	//
	//float weight = 1.0f; //Weight of the vehicle. Affects the recoil from colliding with another vehicle
	//
	//float gravityForce = 180.0f;
	//
	//Vector3f vel; //Velocity that the car has
	//Vector3f currNorm;       //Normal vector of the ground
	//Vector3f currNormSmooth; //'Up' vector to be used in orienting the camera
	//bool onPlane;
	//float inAirTimer = 0.0f;
	//Triangle3D* currentTriangle = nullptr; //The triangle that the vehicle is currently on
	//float canMoveTimer = 0.0f;
	//float deadTimer = -1.0f; //Negative = alive
	//float fallOutTimer = -1.0f; //Negative = alive
	//bool isDead = false; //Gets set after you explode after loosing health
	//
	////Variables used solely for audio
	//Source* sourceEngine = nullptr;
	//Source* sourceStrafe = nullptr;
	//float sourceStrafeTimer = 0; //How long you have been strafing for
	//Source* sourceSlipSlowdown = nullptr;
	//Source* sourceDanger = nullptr;
	//Source* sourceHeal = nullptr;
	//int boostIndex = 0; //Index for boost sound effect
	//
	//Vector3f camDir;       //Direction that the car wants the camera to be facing
	//Vector3f camDirSmooth; //The actual direction the camera will be facing
	//
	//Vector3f camDeathPosition; //Position that the camera stays at when you die
	//
	//float exhaustLength = 0.0f;         //Length of the echaust coming out of the engines (scale)
	//float exhaustLengthTarget = 0.0f;   //What the exhaustLength is trying to be
	//const float exhaustApproach = 5.0f; //How fast the exhaustLength approaches exhaustLengthTarget
	//
	////Variables to keep track of what lap you're on
	//int lastCheckpointID = 0;
	//int currentLap = 0;
	//int lapDistance = 0; //Accumulator for how many checkpoints youve passed through (in the right direction)

	float canMoveTimer = 0.0f;

	//animation variables
	float runAnimationCycle = 0.0f;

	const float displayHeightOffset = 0.6f;
	const float displayBallOffset = 3.0f;

	bool  inputJump;
	bool  inputJumpPrevious;
	bool  inputAction;
	bool  inputActionPrevious;
	bool  inputAction2;
	bool  inputAction2Previous;
	float inputX;
	float inputY;
	float inputX2;
	float inputY2;

	void moveMeGround();

	void moveMeAir();

	void spindash();

	void calcSpindashDirection();

	void setInputs();

	void updateAnimationValues();

public:
	Car();
	Car(float x, float y, float z);
	~Car();

	void step();

	//Call this to "redraw" sonic
	void animate();

    //updates the global camera's values based on 
    // this player's values
    void refreshCamera();

	void setVelocity(float xVel, float yVel, float zVel);

	Vector3f* getVelocity();

	void setCanMoveTimer(float newTimer);

	void startGrinding();

	void stopGrinding();

    void setOnRocket(bool newOnRocket);

	void setIsBall(bool newIsBall);

	void setOnGround(bool newOnGround);

	//called by Rail when sonic jumps off
	void doJump();

	void setRelativeUp(Vector3f* newUp);

	std::list<TexturedModel*>* getModels();

	void loadVehicleInfo();

	static void deleteStaticModels();

	const bool isVehicle();

    Vector3f* getCameraDirection();

    void setCameraDirection(Vector3f* newDirection);

};
#endif
