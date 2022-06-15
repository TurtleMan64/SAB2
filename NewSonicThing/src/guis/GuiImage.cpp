#include <glad/glad.h>

#include "guiimage.hpp"
#include "guimanager.hpp"
#include "../toolbox/vector.hpp"

GuiImage::GuiImage()
{
    
}

GuiImage::GuiImage(GLuint textureId, Vector2f* position, Vector2f* size, float rotation)
{
    this->textureId = textureId;
    this->position.x =   (position->x*2.0f) - 1;
    this->position.y = -((position->y*2.0f) - 1);
    this->sizeRaw.set(size);
    this->sizeScaled.set(size);
    this->visible = true;
    this->scale = 1;
    this->rotation = rotation;
}

GuiImage::GuiImage(GLuint textureId, float posX, float posY, float sizeX, float sizeY, float rotation)
{
    this->textureId = textureId;
    this->position.x =   (posX*2.0f) - 1;
    this->position.y = -((posY*2.0f) - 1);
    this->sizeRaw.x = sizeX;
    this->sizeRaw.y = sizeY;
    this->sizeScaled.x = sizeX;
    this->sizeScaled.y = sizeY;
    this->visible = true;
    this->scale = 1;
    this->rotation = rotation;
}

GuiImage::~GuiImage()
{
    GuiManager::removeImageToRender(this);
}

Vector2f* GuiImage::getPosition()
{
    return &position;
}

Vector2f* GuiImage::getSizeRaw()
{
    return &sizeRaw;
}

void GuiImage::setX(float newX)
{
    position.x = (newX*2.0f) - 1;
}

void GuiImage::setY(float newY)
{
    position.y = -((newY*2.0f) - 1);
}

float GuiImage::getScale()
{
    return scale;
}

void GuiImage::setScale(float newScale)
{
    scale = newScale;
    sizeScaled.x = sizeRaw.x*scale;
    sizeScaled.y = sizeRaw.y*scale;
}

Vector2f* GuiImage::getSizeScaled()
{
    return &sizeScaled;
}

void GuiImage::setSizeScaled(float scaleX, float scaleY)
{
    sizeScaled.x = scaleX;
    sizeScaled.y = scaleY;
}
