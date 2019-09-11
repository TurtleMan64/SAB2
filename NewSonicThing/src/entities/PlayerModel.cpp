#include <glad/glad.h>
#include <vector>
#include <string>

#include "../engineTester/main.h"
#include "entity.h"
#include "playermodel.h"
#include "../toolbox/vector.h"
#include "../particles/particletexture.h"
#include "../menu/timer.h"

void PlayerModel::animate(int, float) {}

void PlayerModel::setOrientation(float, float, float, float, float, float, float, Vector3f*) {}

const float PlayerModel::getDisplayBallOffset() {return 0.0f;}

ParticleTexture* PlayerModel::getBallTexture() {return nullptr;}

void PlayerModel::log(std::list<std::string>* log)
{
    if (Global::mainHudTimer == nullptr)
    {
        return;
    }

    std::string line = std::to_string(Global::mainHudTimer->totalTime) + " " +
                       std::to_string(currentAnimIndex) + " " +
                       std::to_string(currentAnimTime) + " " +
                       std::to_string(position.x) + " " +
                       std::to_string(position.y) + " " +
                       std::to_string(position.z) + " " +
                       std::to_string(rotX) + " " +
                       std::to_string(rotY) + " " +
                       std::to_string(rotZ) + " " +
                       std::to_string(rotRoll) + " " +
                       std::to_string(currentUpDirection.x) + " " +
                       std::to_string(currentUpDirection.y) + " " +
                       std::to_string(currentUpDirection.z) + "\n";

    log->push_back(line);
}
