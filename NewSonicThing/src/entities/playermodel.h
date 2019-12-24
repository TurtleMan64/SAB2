#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

class ParticleTexture;

#include "entity.h"
#include "../toolbox/vector.h"

class PlayerModel : public Entity
{
protected:
    int currentAnimIndex;
    float currentAnimTime;
    Vector3f currentUpDirection;

public:
    virtual void animate(int animIndex, float time);
    
    virtual void setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, Vector3f* up);

    virtual void setBaseColor(float r, float g, float b);

    virtual void setBaseAlpha(float a);

    virtual void setRenderOrder(char newOrder);

    const virtual float getDisplayBallOffset();

    virtual ParticleTexture* getBallTexture();

    //logs a frame to the global race log
    void log();
};
#endif
