#include <glad/glad.h>

#include "entity.h"
#include "playermodel.h"
#include "../toolbox/vector.h"
#include "../particles/particletexture.h"

void PlayerModel::animate(int, float) {}

void PlayerModel::setOrientation(float, float, float, float, float, float, float, Vector3f*) {}

const float PlayerModel::getDisplayBallOffset() {return 0.0f;}

ParticleTexture* PlayerModel::getBallTexture() {return nullptr;}
