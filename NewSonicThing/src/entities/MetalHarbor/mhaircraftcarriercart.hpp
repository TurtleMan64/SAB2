#ifndef MH_AIRCRAFTCARRIERCART
#define MH_AIRCRAFTCARRIERCART

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;
class Dummy;

class MH_AircraftCarrierCart : public CollideableObject
{
private:
    enum states
    {
        MOVING_FORWARD = 0,
        MOVING_BACKWARDS = 1,
        STOPPED = 2
    };
    states stateCurrent = STOPPED;

    //The visual model that doesn't use vertex colors
    static std::vector<TexturedModel*> models;
    //The visual model for the front wheels
    static std::vector<TexturedModel*> modelsWheelFront;
    //The visual model for the back wheels
    static std::vector<TexturedModel*> modelsWheelBack;
    //The visual model that is for textures with transparency
    static std::vector<TexturedModel*> modelsTransparent;
    //The collision model
    static CollisionModel* cmOriginal;

    //Dummy used to be able to display the front wheels model
    Dummy* wheelFront = nullptr;
    //Dummy used to be able to display the back wheels model
    Dummy* wheelBack = nullptr;
    //Dummy used to be able to display the transparent model
    Dummy* bodyTransparent = nullptr;

    //Horizontal Offset of the Front Wheel
    const float WHEEL_OFFSET_FRONT_HORIZONTAL = 8.86459f;
    //Horizontal Offset of the Back Wheel
    const float WHEEL_OFFSET_BACK_HORIZONTAL = -13.1062f;
    //Vertical Offset of the Front Wheel
    const float WHEEL_OFFSET_FRONT_VERTICAL = 4.28373f;
    //Vertical Offset of the Back Wheel
    const float WHEEL_OFFSET_BACK_VERTICAL = 5.71319f;
    //Speed of the Front Wheel
    const float WHEEL_SPEED_FRONT = 2.0f;
    //Speed of the Back Wheel, different because it's a bigger wheel
    const float WHEEL_SPEED_BACK = 1.885f;
    //minimum value of the shake timer
    const float SHAKE_TIMER_MIN = 3.14159265359f;
    //maximum value of the shake timer
    const float SHAKE_TIMER_MAX = 31.4159265359f;

    //used for player collisions with the front and back of the platform when moving
    const float COLLISION_POSITION_SIdEWAYS = 9.9351f;
    const float COLLISION_POSITION_FORWARD = 20.407f;
    const float COLLISION_POSITION_BACKWARD_INNER = -23.0f;
    const float COLLISION_POSITION_BACKWARD_OUTER = -25.0f;
    const float COLLISION_POSITION_BACKSIdEWAYS_INNER = 5.0f;
    const float COLLISION_RADIUS = 5.0f; 
    const float COLLISION_HEIGHT = 11.5f;

    const float MAX_UPDATE_DISTANCE_SQUARED = 1000*1000;

    //if false, the platform doesn't need to update at all after being initialized since it doesn't move
    //set by having a displacementMax of 0
    bool platformMoves = false;
    //initial position of the platform
    Vector3f positionInitial;
    //determines the directions the wheels are rotated
    //in step, changing the direction is a multiplation with -1, so to flip between forwards or backwards at the start, set to -1
    int wheelMovementDirectionMultiplier = 1;
    //the max displacement from the starting position
    float displacementMax = 0.0f;
    //current displacement, used for keeping track of how far the pulley has moved, so you can stop it at the destination
    float displacementCurrent = 0.0f;
    //speed at which the platform moves
    float speed = 0.0f;
    //the direction the platform moves in, used for making movement possible, value calculated
    //in calculateDirectionVector(), called from the constructor
    Vector3f directionVector;
    //keeps track of how long the platform has been stopped for, so it can start moving again
    float stoppedTimer = 0.0f;
    //position the platform stopped at, used for the shaking
    Vector3f positionStopped;
    //used for the shaking of the platform, dt * 20 is added to it to speed up the shaking animation
    float shakeTimer = SHAKE_TIMER_MIN;

    //Functions for the constructor start here

    //Makes the body used for the front wheels model
    inline void setupModelWheelFront();
    //Makes the body used for the back wheels model
    inline void setupModelWheelBack();
    //Makes the body used for the transparent model
    inline void setupModelTransparent();

    //Functions for step() start here
    
    //Moves the platform towards the point specified in the arguments
    inline Vector3f calculateMovementAmount(Vector3f displacementVectorLocal);
    //Ensures the wheels and other separate pieces have their correct positions
    //Relative to last position
    inline void syncBodyPositionsRelative(Vector3f movementAmount);
    //Ensures the wheels and other separate pieces have their correct positions
    //Based on the position of the platform, desync impossible, but more code
    inline void syncBodyPositionsAbsolute();
    //Makes the wheels spin, based on the direction specified by wheelMovementDirectionMultiplier
    inline void spinWheels();
    //Moves the player to keep them in sync with the moving platform
    inline void movePlayer(Vector3f movementAmount);
    //checks if sonic is near the front or back of the platform depending on the active hitboxes determined by the arguments
    //if sonic is close to it, push him away based 
    //sonic will clip through the platform otherwise
    inline void pushSonicAway(bool frontHitboxes, bool backHitboxes);
    //actual code for checking the collision cylinders against sonic's position
    //used in pushSonicAway to determine if sonic is close to the front or back of the platform
    inline bool collisionCheckCylinder(Vector3f pos, float radius, float height);
    //Creates the shaking effect of the platform when it stops
    inline Vector3f shakePlatform();


public:
    MH_AircraftCarrierCart();

    MH_AircraftCarrierCart(float x, float y, float z, float dirX, float dirZ, float displacementMax, float speed);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
