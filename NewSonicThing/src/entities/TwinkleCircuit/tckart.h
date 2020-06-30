#ifndef TC_KART
#define TC_KART

class TexturedModel;
class Triangle3D;
class ParticleTexture;
class Source;
class Dummy;

#include <list>
#include "../entity.h"
#include "../../toolbox/vector.h"

struct EngineExhaust
{
    int textureID;
    float posUp;
    float posAt;
    float posSide;
    float lengthScale;
    float size;
};

struct MachineStats
{
    float terminalSpeed;      //Normal speed cap
    float terminalAccelGas;   //How fast you accelerate towards terminal speed
    float terminalAccelBrake; //How fast you accelerate towards 0 while braking
    float terminalAccelCoast; //How fast you accelerate towards 0 while coasting

    float turnSpeed;  //How fast you turn
    float turnPunish; //How much you slow down due to turning

    float diveSpeed;  //How fast you dive both up and down 
    float divePunish; //How much you are punished for pulling up
    
    float strafePercentage;     //Percent that strafing goes sideways
    float strafeTerminalPunish; //Percent that strafing lowers the terminal speed
    
    float slipTimerMax;       //What the slipTimer gets set to when you trigger a slip
    float slipThreshold;      //Point at which you trigger a slip
    float slipAngleAccel;     //How fast the slip angle approaches the target
    float slipAngleMax;       //Max that the angle can slip by
    float slipPower;          //How much you turn when you slip turn
    float slipNegativePower;  //Multiplier for when you lose speed during slipping
    float slipPositivePower;  //Multiplier for when you gain speed during slipping
    float slipTimerThreshold; //Time threshold for when you start loosing speed vs gaining it during slipping
    
    float spinTimeMax;   //How long the spin attack lasts
    float spinTimeDelay; //How long until you can do another spin attack
    
    float sideAttackTimeMax; //How long you side attack for
    float sideAttackSpeed;   //How fast you move during the attack at the beginning
    
    float boostSpeed;        //Max speed when boosting
    float boostKick;         //Initial speed addition you get from starting a boost
    float boostDuration;     //How long you boost for
    float boostDelayMax;     //Time until you can boost again
    float boostHealthPunish; //How much health you loose for a boost

    float healRate;            //Health per second that you recover while in health pads
    float hitWallHealthPunish; //How much health you loose when hitting a wall perpendicularly at max speed
    
    float weight; //Weight of the vehicle. Affects the recoil from colliding with another vehicle
    float gravityForce;

    int boostIndex; //Index for boost sound effect
};

class TC_Kart : public Entity
{
private:
    static std::list<TexturedModel*> models;
    static std::list<EngineExhaust> exhausts;
    static MachineStats machineBaseStats;
    static std::list<TexturedModel*> modelsEngine;

    const float VEL_SLOWEST = 0.0006f; //Slowest that the car can move
    const float FLOOR_OFFSET = 0.1f; //How much you stay away from the floor

    const float smoothTransitionThreshold = 0.6f; //This should not change bewteen racers
    const float surfaceTension = 10.0f;     //To not fly off the track
    const float hitWallTimePunish = 0.125f; //How long you can't move after hitting a wall

    const float camAngleLookdown = 0.3f;    //How much the camera looks down
    const float camRadiusTarget  = 40.5;    //Camera radius at zero speed
          float camRadius        = 60.0f;
    const float camRadiusAdjust  = 0.0005f; //How much the camera zooms out due to speed
    const float camFovAdjust     = 0.05f;   
    const float camHeightOffset  = 9.0f;    //Point above the vehicle that the camera focuses on
    const float camSmoothness    = 5.0f;    //Smoothes currNormSmooth and camDirSmooth

    float terminalSpeed      = 298.0f; //Normal speed cap
    float terminalAccelGas   = 0.45f;  //How fast you accelerate towards terminal speed
    float terminalAccelBrake = 1.0f;   //How fast you accelerate towards 0 while braking
    float terminalAccelCoast = 0.1f;   //How fast you accelerate towards 0 while coasting

    float turnSpeed  = 1.38f; //How fast you turn
    float turnPunish = 0.01f; //How much you slow down due to turning

    float diveSpeed  = 4.5f; //How fast you dive both up and down 
    float divePunish = 2.5f; //How much you are punished for pulling up

    float strafePercentage     = 0.2f;  //Percent that strafing goes sideways
    float strafeTerminalPunish = 0.95f; //Percent that strafing lowers the terminal speed

    float slipTimer          = 0.0f;
    float slipAccumulated    = 0.0f;  //How much slip you have accumulated from jerking the wheel
    float slipTimerMax       = 0.75f; //What the slipTimer gets set to when you trigger a slip
    float slipThreshold      = 4.5f;  //Point at which you trigger a slip
    float slipAngle          = 0.0f;  //Current angle that you are facing during the slip
    float slipAngleTarget    = 0.0f;  //What the slip angle is approaching
    float slipAngleAccel     = 5.0f;  //How fast the slip angle approaches the target
    float slipAngleMax       = 40.0f; //Max that the angle can slip by
    float slipPower          = 1.5f;  //How much you turn when you slip turn
    float slipNegativePower  = 1.1f;  //Multiplier for when you lose speed during slipping
    float slipPositivePower  = 0.75f; //Multiplier for when you gain speed during slipping
    float slipTimerRight     = 0.0f;  //How long you have been slipping to the right for
    float slipTimerLeft      = 0.0f;  //How long you have been slipping to the left for
    float slipTimerThreshold = 0.5f;  //Time threshold for when you start loosing speed vs gaining it during slipping

    float spinTimeMax   = 1.65f; //How long the spin attack lasts
    float spinTimeDelay = 2.5f;  //How long until you can do another spin attack
    float spinTimer     = 0.0f;

    float sideAttackTimer   = 0.0f;
    float sideAttackDir     = 1.0f;    //Direction of the side attack
    float sideAttackTimeMax = 0.1666f; //How long you side attack for
    float sideAttackSpeed   = 150.0f;  //How fast you move during the attack at the beginning

    float boostSpeed        = 462.0f; //Max speed when boosting
    float boostKick         = 0.7f;   //Initial speed addition you get from starting a boost
    float boostDuration     = 1.0f;   //How long you boost for
    float boostDelayMax     = 1.15f;  //Time until you can boost again
    float boostDelayTimer   = 0.0f;
    float boostHealthPunish = 0.2f;   //How much health you loose for a boost

          float camAngleAdditionalLookdown                 = 0.0f; //Additional lookdown due to driving through a steep concave or convex area
    const float camAngleAdditionalLookdownSmoothness       = 0.4f; //How fast the lookdown approaches the target
          float camAngleAdditionalLookdownTarget           = 0.0f;
    const float camAngleAdditionalLookdownTargetSmoothness = 2.5f; //How fast the target approaches 0
    const float camAngleAdditionalLookdownMax              = 0.3f; //Max value of the lookdown
    const float camAngleAdditionalLookdownScale            = 0.005f; //How much the target gets added to by each new triangle collision

    float health   = 1.0f;
    float healRate = 0.666f; //Health per second that you recover while in health pads
    float hitWallHealthPunish = 0.3f; //How much health you loose when hitting a wall perpendicularly at max speed

    float weight = 1.0f; //Weight of the vehicle. Affects the recoil from colliding with another vehicle

    float gravityForce = 180.0f;
public:
    Vector3f vel; //Velocity that the car has
    Vector3f currNorm;       //Normal vector of the ground
private:
    Vector3f currNormAnim;   //normal for drawing the car
    Vector3f currNormSmooth; //'Up' vector to be used in orienting the camera
    bool onPlane;
    float inAirTimer = 0.0f;
    Triangle3D* currentTriangle = nullptr; //The triangle that the vehicle is currently on
    float canMoveTimer = 0.0f;
    float deadTimer = -1.0f; //Negative = alive
    float fallOutTimer = -1.0f; //Negative = alive
    bool isDead = false; //Gets set after you explode after loosing health
    int ringsUntilBoost = 15; //How many more rings you need to boost
    int prevRingCount = 0;

    bool playerIsDrivingMe = false;

    //Variables used solely for audio
    Source* sourceEngine = nullptr;
    Source* sourceStrafe = nullptr;
    float sourceStrafeTimer = 0; //How long you have been strafing for
    Source* sourceSlipSlowdown = nullptr;
    Source* sourceDanger = nullptr;
    Source* sourceHeal = nullptr;
    int boostIndex = 0; //Index for boost sound effect

    Vector3f camDir;       //Direction that the car wants the camera to be facing
    Vector3f camDirSmooth; //The actual direction the camera will be facing

    Vector3f camDeathPosition; //Position that the camera stays at when you die

    float exhaustLength = 0.0f;         //Length of the echaust coming out of the engines (scale)
    float exhaustLengthTarget = 0.0f;   //What the exhaustLength is trying to be
    const float exhaustApproach = 5.0f; //How fast the exhaustLength approaches exhaustLengthTarget

    //Variables to keep track of what lap you're on
    int lastCheckpointID = 0;
    int currentLap = 0;
    int lapDistance = 0; //Accumulator for how many checkpoints youve passed through (in the right direction)

    Dummy* engineLeft  = nullptr;
    Dummy* engineRight = nullptr;
    float engineLeftSpin = 0.0f;
    float engineRightSpin = 0.0f;

    bool  inputGas;
    bool  inputBrake;
    bool  inputBoost;
    bool  inputAttackSpin;
    bool  inputAttackSide;
    float inputWheel;
    float inputWheelJerk;
    float inputDive;
    float inputL;
    float inputR;

    bool inputGasPrevious;
    bool inputBrakePrevious;
    bool inputBoostPrevious;
    bool inputAttackSpinPrevious;
    bool inputAttackSidePrevious;

    void setInputs();

    void createEngineParticles(Vector3f* initPos, Vector3f* endPos, float initialScale, int count);

    void checkpointTest();

    void explode();

public:
    TC_Kart();
    TC_Kart(float x, float y, float z, float xDir, float yDir, float zDir);
    ~TC_Kart();

    void step();

    void giveMeABoost();

    void giveMeAJump();

    void setVelocity(float xVel, float yVel, float zVel);

    void setLapDistance(int newDistance);

    void setCanMoveTimer(float newTimer);

    std::list<TexturedModel*>* getModels();

    void loadVehicleInfo();

    static void deleteStaticModels();

    const bool isVehicle();
};
#endif
