#ifndef MH_CRATEPLATFORM
#define MH_CRATEPLATFORM

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;
class Body;
class Source;

class MH_CratePlatform : public CollideableObject
{
private:
    enum states
    {
        MOVING_FORWARD,
        MOVING_BACKWARDS,
        STOPPED
    };
    int stateCurrent;

    enum types
    {
        LONG,
        CRATE,
        CRATELESS
    };
    int platformType;

    //The visual model of the long crate platform
    static std::list<TexturedModel*> modelsCratePlatformLong;
    static std::list<TexturedModel*> modelsCratePlatformCrate;
    static std::list<TexturedModel*> modelsCratePlatformNoCrate;

    //The collision model
    static CollisionModel* cmCratePlatformLong;
    static CollisionModel* cmCratePlatformCrate;
    static CollisionModel* cmCratePlatformNoCrate;

    //minimum value of the shake timer
    const float SHAKE_TIMER_MIN = 3.14159265359f;
    //maximum value of the shake timer
    const float SHAKE_TIMER_MAX = 31.4159265359f;

    //this set is for the long crate platform
    const float COL_LONGPLAT_POS_FORWARD_WITH_BAR = 31.3f;
    const float COL_LONGPLAT_POS_FORWARD_WO_BAR = 26.27f;
    const float COL_LONGPLAT_POS_SIDE_WITH_BAR = 5.15f;
    const float COL_LONGPLAT_POS_SIDE_WO_BAR = 5.29f;
    const float COL_LONGPLAT_RADIUS_SIDE_WITH_BAR = 6.35f;
    const float COL_LONGPLAT_RADIUS_SIDE_WO_BAR = 5.29f;

    const float COLLISION_RADIUS = 5.0f; 
    const float COLLISION_HEIGHT = 11.5f;

    //if false, the platform doesn't need to update at all after being initialized since it doesn't move
    //set by having a displacementMax of 0
    bool platformMoves;
    //initial position of the platform
    Vector3f positionInitial;
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

    //The audio source used to play the platforms's sound effect
    Source* platformAudioSource = nullptr;

    //Functions for step() start here
    
    //Moves the platform towards the point specified in the arguments
    inline Vector3f calculateMovementAmount(Vector3f displacementVectorLocal);
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
    //plays the noise the platform makes when it stops
    inline void playStoppingSound();
    //resets the audio source so the noise can play again
    inline void resetStoppingSound();

public:
    MH_CratePlatform();

    MH_CratePlatform(float x, float y, float z, float dirX, float dirZ, float displacementMax, float speed, int type);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
