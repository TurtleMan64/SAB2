#ifndef PULLEY
#define PULLEY

#include <list>
#include "entity.hpp"
#include "collideableobject.hpp"

class TexturedModel;
class Body;
class Source;

class Pulley : public CollideableObject
{
private:
    //Visual model of the handle.
    static std::vector<TexturedModel*> modelsHandle;
    //Visual model of the rope.
    static std::vector<TexturedModel*> modelsRope;
    //Visual model of the top of the pulley.    
    static std::vector<TexturedModel*> modelsTop;
    //Collision model of the top of the pulley.
    static CollisionModel* cmTop;

    //The entity of the rope.
    Body* rope = nullptr;
    //The entity of the top of the rocket.
    Body* top = nullptr;

    //The y position of the top of the pulley, this is useful to have so the position of the handle can
    //be saved as a displacement from this rather than a separate vector3f
    float pulleyTopYPosition = 0.0f;

    //The players velocity is set to this to make the autocam point towards the direction the pulley is pointing
    //Also ends up rotating the player to face the correct direction as well
    Vector3f cameraDirectionVector;

    //How far down the handle is from the top of the rocket.
    //Used for both the position of the handle and the length of the rope.
    //This gets moved dependi
    float handleVerticalDisplacement = 0.0f;
    //The very bottom point of how far
    float handleVerticalDisplacementBottom = 0.0f;
    //The closest the handle can get to the top of the pulley
    static constexpr float HANDLE_VERTICAL_DISPLACEMENT_MINIMUM = 10;

    //Radius of the horizontal component of the hitbox of the handle
    static constexpr float HITBOX_RADIUS = 5;
    //Height of the hitbox of the handle
    static constexpr float HITBOX_HEIGHT = 10;

    //Player is riding the pulley
    bool playerIsOnPulley = false;

    //The audio source used to play the Pulley's audio
    Source* pulleyAudioSource = nullptr;

    //Max value the bob timer can have, it will be clamped at this point
    static constexpr float BOB_TIMER_MAX = 31.4159265359f;
    //Minimum value the bob timer can have, since you divide by this value so it can't be set to 0
    static constexpr float BOB_TIMER_MIN = 3.14159265359f;
    //Timer used for the animation of the Pulley bobbing up and down
    float bobTimer = BOB_TIMER_MAX;
    //Whether the pulley is bobbing or not
    bool isBobbing = false;

    //Bool used to give a name to the bool passed to the move_pulley function, purely to make it easier
    //to read
    static constexpr bool MOVE_UP = true;
    //Bool used to give a name to the bool passed to the move_pulley function, purely to make it easier
    //to read
    static constexpr bool MOVE_DOWN = false;
    
    //The speed the pulley moves at
    float PULLEY_SPEED = 150;

    //functions for the constructor start here
    
    //Get the vector for the horizontal direction the camera should point using the y rotation
    inline Vector3f calculateCameraDirectionVector() const;
    //Setup the visual model of the rope of the pulley as a separate entity, 
    //allowing it to move separately from the handle.
    inline void setupPulleyRope();
    //Setup the visual model and collision of the top of the pulley as a separate entity, 
    //allowing it to move separately from the handle.
    inline void setupPulleyTop();

    //functions for step() start here

    //Check if the player has collided with the rocket handle.
    inline bool playerWithinHandleHitbox();
    //Check if the pulley is at the bottom position where it can be grabbed.
    inline bool handleAtBottom() const;
    //Check if the pulley is at the very top where it stops moving.
    inline bool handleAtTop() const;
    //Play and loop the sound the pulley makes while it moves.
    inline void playPulleySound();
    //Stop playing the pulley sound.
    inline void stopPulleySound();
    //Move the pulley either up or down, depending on if true or false
    inline void movePulley(bool movePulleyUp);
    //Returns either 1 to move down or -1 to move up, used to multiply the speed to control direction
    //and reduce duplicate code.
    inline int getPulleyMoveDirection(bool movePulleyUp) const;
    //Check if the player has jumped.
    inline bool jumpInputPressed() const;
    //Handle the animation of the Pulley bobbing up and down when it hits the top with the player on it
    inline void bobPulley();

public:
    Pulley();
    Pulley(float x, float y, float z, float rotY, float handleVerticalDisplacement);
    ~Pulley();

    void step();

    std::vector<TexturedModel*>* getModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
