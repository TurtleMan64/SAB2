#ifndef CONTROLLABLEPLAYER_H
#define CONTROLLABLEPLAYER_H

#include "entity.hpp"
#include "../toolbox/vector.hpp"

class ShieldMagnet;
class ShieldGreen;

class ControllablePlayer : public Entity
{
protected:
    bool inWater = false;
    bool inWaterPrevious = false;
    float waterHeight = 0.0f;

public:
    Vector3f vel;
    Vector3f camDir;
    Vector3f camDirSmooth;
    Vector3f relativeUp;
    bool onGround = false;
    float canMoveTimer = 0.0f;

    //Functions that every player must be able to handle in some way
public:
    virtual void goUp();

    virtual void jump();

    virtual void hitDashpad();

    virtual void startGrinding();

    virtual void stopGrinding();

    virtual void animate();

    virtual void refreshCamera();

    virtual void setCanMoveTimer(int newMoveTimer);

    virtual void setHoverTimer(float newTime);

    virtual float getHoverTimer();

    virtual void setInWater(float newWaterHeight);

    virtual void takeDamage(Vector3f* damageSource);

    virtual void rebound(Vector3f* source);

    virtual bool isVulnerable();

    virtual bool canDealDamage();

    virtual bool canBreakObjects();

    virtual void die();

    virtual bool isDying();

    //Returns the center position of where the player is being drawn.
    virtual Vector3f getCenterPosition();

    virtual ShieldMagnet* getShieldMagnet();

    virtual void setShieldMagnet(ShieldMagnet* newMagnet);

    virtual ShieldGreen* getShieldGreen();

    virtual void setShieldGreen(ShieldGreen* newGreen);

    virtual bool canGrabDiggablePiece();

    virtual bool canReadHint();

    virtual void setReadingHint(bool isReadingHint);

    virtual void increaseCombo();

    virtual void setInvincibleTimer(float newTimer);

    virtual void setSpeedshoesTimer(float newTimer);

    virtual void startGrabbing();

    virtual void setIsDriving(bool isDriving);

    virtual void stopGrabbing();

    virtual float getDiggingTimer();

    virtual void hitSpring(Vector3f* direction, float power, float lockInputTime, bool resetsCamera);

    virtual void hitSpringTriple(Vector3f* direction, float power, float lockInputTime);

    virtual void hitSpringYellow(const Vector3f* direction, float power, float lockInputTime);

    virtual void hitSpeedRamp(Vector3f* direction, float speed, float lockInputTime);

    virtual void jumpOffPulley(Vector3f forwardDirectionVector);
};
#endif
