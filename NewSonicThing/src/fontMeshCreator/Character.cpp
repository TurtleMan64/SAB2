#include "character.hpp"

Character::Character(int id, float xTextureCoord, float yTextureCoord, float xTexSize, float yTexSize,
    float xOffset, float yOffset, float sizeX, float sizeY, float xAdvance)
{
    this->id = id;
    this->xTextureCoord = xTextureCoord;
    this->yTextureCoord = yTextureCoord;
    this->xOffset = xOffset;
    this->yOffset = yOffset;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->xMaxTextureCoord = xTexSize + xTextureCoord;
    this->yMaxTextureCoord = yTexSize + yTextureCoord;
    this->xAdvance = xAdvance;
}

Character::Character()
{
    this->id = -1;
    this->xTextureCoord = 0;
    this->yTextureCoord = 0;
    this->xOffset = 0;
    this->yOffset = 0;
    this->sizeX = 0;
    this->sizeY = 0;
    this->xMaxTextureCoord = 0 + 0;
    this->yMaxTextureCoord = 0 + 0;
    this->xAdvance = 0;
}

int Character::getId()
{
    return id;
}

float Character::getxTextureCoord()
{
    return xTextureCoord;
}

float Character::getyTextureCoord()
{
    return yTextureCoord;
}

float Character::getXMaxTextureCoord()
{
    return xMaxTextureCoord;
}

float Character::getYMaxTextureCoord()
{
    return yMaxTextureCoord;
}

float Character::getxOffset()
{
    return xOffset;
}

float Character::getyOffset()
{
    return yOffset;
}

float Character::getSizeX()
{
    return sizeX;
}

float Character::getSizeY()
{
    return sizeY;
}

float Character::getxAdvance()
{
    return xAdvance;
}
