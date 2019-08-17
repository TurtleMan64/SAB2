#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

class ParticleTexture;

#include "entity.h"
#include "../toolbox/vector.h"


class PlayerModel : public Entity
{
public:
	virtual void animate(int animIndex, float time);
    
    virtual void setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, Vector3f* up);

    const virtual float getDisplayBallOffset();

    virtual ParticleTexture* getBallTexture();
};
#endif
