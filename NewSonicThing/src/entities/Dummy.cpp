#include <list>

#include "../toolbox/vector.h"
#include "dummy.h"

Dummy::Dummy(std::list<TexturedModel*>* models)
{
    myModels = models;
    scale = 1;
    visible = true;
}

void Dummy::step()
{

}

void Dummy::setRotation(float xr, float yr, float zr, float sr)
{
    rotX = xr;
    rotY = yr;
    rotZ = zr;
    rotRoll = sr;
}

std::list<TexturedModel*>* Dummy::getModels()
{
    return myModels;
}

void Dummy::setModels(std::list<TexturedModel*>* newModels)
{
    myModels = newModels;
}
