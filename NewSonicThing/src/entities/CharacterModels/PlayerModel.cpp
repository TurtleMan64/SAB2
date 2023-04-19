#include <glad/glad.h>
#include <vector>
#include <string>

#include "../../engineTester/main.hpp"
#include "../entity.hpp"
#include "playermodel.hpp"
#include "../../toolbox/vector.hpp"
#include "../../particles/particletexture.hpp"
#include "../../menu/timer.hpp"
#include "../raceghost.hpp"

void PlayerModel::animate(int, float) {}

void PlayerModel::setOrientation(float, float, float, float, float, float, float, const Vector3f*) {}

void PlayerModel::setBaseColor(float, float, float) {}

void PlayerModel::setBaseAlpha(float) {}

void PlayerModel::setBaseVisibility(bool) {}

void PlayerModel::setRenderOrderOverride(char) {}

float PlayerModel::getDisplayBallOffset() const {return 0.0f;}

ParticleTexture* PlayerModel::getBallTexture() {return nullptr;}

void PlayerModel::log()
{
    if (Global::mainHudTimer == nullptr || Global::raceLogSize >= Global::raceLogSizeMax)
    {
        return;
    }

    GhostFrame frame;
    frame.time = Global::mainHudTimer->totalTime;
    frame.animIndex = currentAnimIndex;
    frame.animTime = currentAnimTime;
    frame.pos.set(&position);
    //frame.pos = frame.pos + currentUpDirection.scaleCopy(6.0f); //todo: doesnt work on kart??
    frame.rot.set(rotX, rotY, rotZ, rotRoll);
    frame.up.set(&currentUpDirection);

    Global::raceLog[Global::raceLogSize] = frame;
    Global::raceLogSize++;
}
