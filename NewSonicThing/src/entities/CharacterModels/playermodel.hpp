#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

class ParticleTexture;

#include "../entity.hpp"
#include "../../toolbox/vector.hpp"

class PlayerModel : public Entity
{
protected:
    int currentAnimIndex;
    float currentAnimTime;
    Vector3f currentUpDirection;
    bool baseVisible;

public:
    virtual void animate(int animIndex, float time);
    
    virtual void setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, const Vector3f* up);

    virtual void setBaseColor(float r, float g, float b);

    virtual void setBaseAlpha(float a);

    virtual void setBaseVisibility(bool visible);

    virtual void setRenderOrderOverride(char newOrder);

    virtual float getDisplayBallOffset() const;

    virtual ParticleTexture* getBallTexture();

    //logs a frame to the global race log
    void log();
};
#endif
