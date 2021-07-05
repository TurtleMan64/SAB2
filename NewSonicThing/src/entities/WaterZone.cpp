#include <glad/glad.h>

#include "entity.h"
#include "../toolbox/vector.h"
#include "waterzone.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../entities/camera.h"

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

std::list<TexturedModel*>* WaterZone::getModels()
{
    return nullptr;
}
