#include "pulley.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "../engineTester/main.hpp"
#include "../models/models.hpp"
#include "../loading/modelloader.hpp"
#include "../toolbox/maths.hpp"
#include "../collision/collisionmodel.hpp"
#include "../collision/collisionchecker.hpp"
#include "../animation/body.hpp"
#include "../entities/controllableplayer.hpp"
#include "../audio/audioplayer.hpp"
#include "../audio/source.hpp"
#include "../toolbox/input.hpp"

extern float dt;

std::vector<TexturedModel*> Pulley::modelsHandle;
std::vector<TexturedModel*> Pulley::modelsRope;
std::vector<TexturedModel*> Pulley::modelsTop;
CollisionModel* Pulley::cmTop = nullptr;

Pulley::Pulley() {}

Pulley::Pulley(float x, float y, float z, float newRotY, float handleVerticalDisplacement)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = newRotY;
    rotZ = 0;

    scale = 1;
    visible = true;

    this->handleVerticalDisplacement = handleVerticalDisplacement;
    handleVerticalDisplacementBottom = handleVerticalDisplacement;

    cameraDirectionVector = calculateCameraDirectionVector();

    setupPulleyRope();

    setupPulleyTop();
    
    pulleyTopYPosition = position.y;

    //Move pulley down to initial position
    position.y -= handleVerticalDisplacement;

    updateTransformationMatrix();
    //stretch the rope out to where the pulley is
    rope->updateTransformationMatrix(1, handleVerticalDisplacement, 1);
    //the top never moves or changes so it won't be updated again.
    top->updateTransformationMatrix();
    cmTop->transformModel(collideModelTransformed, top->getPosition());
}

void Pulley::step() 
{
    //Idea: make pulley bob when it hits the top? like SA2

    //Change all of this so the start moving is separate from the actual moving, isMoving variable required
    if (playerIsOnPulley)
    {
        if (!isBobbing and !handleAtTop()) 
        {
            //Pulley not yet at top and player is riding, move towards top

            playPulleySound();

            movePulley(MOVE_UP);

            //Reset timer here, the only way this block would ever not be called first is with a handle displacement equal to the 
            bobTimer = BOB_TIMER_MIN;
        }
        else
        {
            //Make sure the pulley doesn't go any closer than the minimum
            //handleVerticalDisplacement = HANDLE_VERTICAL_DISPLACEMENT_MINIMUM;
            isBobbing = true;

            bobPulley();

            stopPulleySound();
        }

        //update the position of the pulley handle so the player can be synced to it
        position.y = pulleyTopYPosition - handleVerticalDisplacement;

        //Make player attach to the pulleys position and not move
        Global::gameMainPlayer->setPosition(position.x, position.y - 5.5f, position.z);
        Global::gameMainPlayer->vel.set(cameraDirectionVector.x, 0, cameraDirectionVector.z);

        if (jumpInputPressed()) //get off the pulley, should also happen if damaged
        {
            Global::gameMainPlayer->vel.set(0,0,0);
            Global::gameMainPlayer->stopGrabbing();
            Global::gameMainPlayer->jumpOffPulley(cameraDirectionVector);

            if (!handleAtTop())
            {
                Global::gameMainPlayer->vel.y += PULLEY_SPEED;
            }

            playerIsOnPulley = false;

            isBobbing = false;
        }
    }
    else if (playerWithinHandleHitbox() && handleAtBottom()) //Player gets on the handle
    {
        //Set the pulley to the state of having the player on it
        playerIsOnPulley = true;

        //This below is all just setting up player variables in preparation for the movement
        Global::gameMainPlayer->startGrabbing();

        //Make player face the right direction
        Global::gameMainPlayer->rotY = (rotY);
    }
    else if (!handleAtBottom())
    {
        //Pulley not yet at bottom and player not on, move towards bottom

        playPulleySound();

        movePulley(MOVE_DOWN);
    }
    else
    {
        //Make sure the pulley doesn't go any father than the maximum displacement
        handleVerticalDisplacement = handleVerticalDisplacementBottom;
        stopPulleySound();
    }

    position.y = pulleyTopYPosition - handleVerticalDisplacement;

    updateTransformationMatrix();
    //stretch the rope out to where the pulley is
    rope->updateTransformationMatrix(1, handleVerticalDisplacement, 1);
    Global::gameMainPlayer->animate();
    Global::gameMainPlayer->refreshCamera();
}

std::vector<TexturedModel*>* Pulley::getModels()
{
    return &Pulley::modelsHandle;
}

void Pulley::loadStaticModels()
{
    if (Pulley::modelsHandle.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Pulley static models...\n");
    #endif

    ModelLoader::loadModel(&Pulley::modelsHandle, "res/Models/Objects/Pulley/", "PulleyHandle");
    ModelLoader::loadModel(&Pulley::modelsRope,   "res/Models/Objects/Pulley/", "PulleyRope");
    ModelLoader::loadModel(&Pulley::modelsTop,    "res/Models/Objects/Pulley/", "PulleyTop");

    if (Pulley::cmTop == nullptr)
    {
        Pulley::cmTop = ModelLoader::loadCollisionModel("Models/Objects/Pulley/", "PulleyTopCollision");
    }
}

void Pulley::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Pulley static models...\n");
    #endif

    Entity::deleteModels(&Pulley::modelsHandle);
    Entity::deleteModels(&Pulley::modelsRope);
    Entity::deleteModels(&Pulley::modelsTop);
}

inline Vector3f Pulley::calculateCameraDirectionVector() const
{
    return Vector3f(sin(Maths::toRadians(rotY)) * 1000, 0, -cos(Maths::toRadians(rotY)) * 1000);
}

inline void Pulley::setupPulleyRope()
{
    rope = new Body(&Pulley::modelsRope); INCR_NEW("Entity");
    rope->visible = (true);
    Global::addEntity(rope);
    rope->setPosition(&position);
}

inline void Pulley::setupPulleyTop()
{
    top = new Body(&Pulley::modelsTop); INCR_NEW("Entity");
    top->visible = (true);
    Global::addEntity(top);
    top->setPosition(&position);
    top->rotY = (rotY);

    collideModelOriginal = Pulley::cmTop;
    collideModelTransformed = collideModelOriginal->duplicateMe();
    CollisionChecker::addCollideModel(collideModelTransformed);
    updateCollisionModel();
}

inline bool Pulley::playerWithinHandleHitbox()
{
    Vector3f playerPulleyDistance = Global::gameMainPlayer->position - position;
    float playerPulleyDistanceHorizontalSquared = (playerPulleyDistance.x)*(playerPulleyDistance.x) + 
                                                  (playerPulleyDistance.z)*(playerPulleyDistance.z);
    return (playerPulleyDistanceHorizontalSquared <= (HITBOX_RADIUS*HITBOX_RADIUS) &&
            std::abs(playerPulleyDistance.y) <= HITBOX_HEIGHT);
}

inline bool Pulley::handleAtBottom() const
{
    return handleVerticalDisplacement >= handleVerticalDisplacementBottom;
}

inline bool Pulley::handleAtTop() const
{
    return handleVerticalDisplacement <= HANDLE_VERTICAL_DISPLACEMENT_MINIMUM;
}

inline void Pulley::playPulleySound()
{
    if (pulleyAudioSource == nullptr)
    {
        pulleyAudioSource = AudioPlayer::play(62, top->getPosition(), 1, false);
    }
    pulleyAudioSource->setLooping(true);
}

inline void Pulley::stopPulleySound()
{
    if (pulleyAudioSource != nullptr)
    {
        if (pulleyAudioSource->isPlaying())
        {
            pulleyAudioSource->stop();
            pulleyAudioSource = nullptr;
        }
    }
}

inline void Pulley::movePulley(bool movePulleyUp)
{
    //if true, the pulley moves up, if false, it moves down
    
    //is either 1 or -1, used to control what direction the pulley moves
    int pulleyDirection = getPulleyMoveDirection(movePulleyUp);

    handleVerticalDisplacement += PULLEY_SPEED * pulleyDirection * dt;
}

inline int Pulley::getPulleyMoveDirection(bool movePulleyUp) const
{
    if (movePulleyUp)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

inline bool Pulley::jumpInputPressed() const
{
    return Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1;
}

inline void Pulley::bobPulley()
{
    if (bobTimer > BOB_TIMER_MAX)
    {
        bobTimer = BOB_TIMER_MAX;
        handleVerticalDisplacement = HANDLE_VERTICAL_DISPLACEMENT_MINIMUM;
        return;
    }
    //Check out the curve of this in a graphing calculator, it's a sine wave that slows down, pretty cool
    handleVerticalDisplacement = HANDLE_VERTICAL_DISPLACEMENT_MINIMUM - (sin(bobTimer) / bobTimer) * 10;

    bobTimer += dt * 20;
}

bool Pulley::canHomingAttackOn() const
{
    return handleAtBottom();
}

Vector3f Pulley::getHomingCenter() const
{
    return position;
}
