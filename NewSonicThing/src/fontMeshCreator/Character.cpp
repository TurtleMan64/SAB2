#include "character.h"

Character::Character(int id, double xTextureCoord, double yTextureCoord, double xTexSize, double yTexSize,
	double xOffset, double yOffset, double sizeX, double sizeY, double xAdvance)
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

double Character::getxTextureCoord()
{
	return xTextureCoord;
}

double Character::getyTextureCoord()
{
	return yTextureCoord;
}

double Character::getXMaxTextureCoord()
{
	return xMaxTextureCoord;
}

double Character::getYMaxTextureCoord()
{
	return yMaxTextureCoord;
}

double Character::getxOffset()
{
	return xOffset;
}

double Character::getyOffset()
{
	return yOffset;
}

double Character::getSizeX()
{
	return sizeX;
}

double Character::getSizeY()
{
	return sizeY;
}

double Character::getxAdvance()
{
	return xAdvance;
}
