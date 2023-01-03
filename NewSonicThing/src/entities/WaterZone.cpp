#include <glad/glad.h>

#include "entity.hpp"
#include "../toolbox/vector.hpp"
#include "waterzone.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../entities/camera.hpp"

#include <list>

class TexturedModel;

WaterZone::WaterZone()
{

}

WaterZone::WaterZone(float x, float y, float z, float xw, float yw, float zw)
{
    position.set(x, y, z);
    width.set(xw, yw, zw);
    visible = false;
}

void WaterZone::step()
{
    Vector3f diff = Global::gameMainPlayer->position - position;
    
    if (diff.x > 0 && diff.x < width.x &&
        diff.y > 0 && diff.y < width.y &&
        diff.z > 0 && diff.z < width.z)
    {
        Global::gameMainPlayer->setInWater(position.y + width.y);
    }

    diff = Global::gameCamera->eye - position;

    if (diff.x > 0 && diff.x < width.x &&
        diff.y > 0 && diff.y < width.y &&
        diff.z > 0 && diff.z < width.z)
    {
        Global::gameCamera->inWater = true;
    }
}

std::vector<TexturedModel*>* WaterZone::getModels()
{
    return nullptr;
}
