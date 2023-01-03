#include <list>

#include "../toolbox/vector.hpp"
#include "dummy.hpp"

Dummy::Dummy(std::vector<TexturedModel*>* models)
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

std::vector<TexturedModel*>* Dummy::getModels()
{
    return myModels;
}

void Dummy::setModels(std::vector<TexturedModel*>* newModels)
{
    myModels = newModels;
}
