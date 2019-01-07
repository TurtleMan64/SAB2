#include <glad/glad.h>
#include <iostream>

#include "modeltexture.h"
#include "../renderEngine/renderEngine.h"

ModelTexture::ModelTexture()
{
	this->shineDamper = 20;
	this->reflectivity = 0.02f;
	this->hasTransparency = 0;
	this->useFakeLighting = 0;
	this->glowAmount = 0;
}

ModelTexture::ModelTexture(GLuint texID)
{
	this->texID = texID;
	this->shineDamper = 20;
	this->reflectivity = 0.02f;
	this->hasTransparency = 0;
	this->useFakeLighting = 0;
	this->glowAmount = 0;
}

GLuint ModelTexture::getID()
{
	return texID;
}

void ModelTexture::setID(GLuint newID)
{
	texID = newID;
}

float ModelTexture::getShineDamper()
{
	return shineDamper;
}
void ModelTexture::setShineDamper(float newShineDamper)
{
	shineDamper = newShineDamper;
}

float ModelTexture::getReflectivity()
{
	return reflectivity;
}
void ModelTexture::setReflectivity(float newReflectivity)
{
	reflectivity = newReflectivity;
}

int ModelTexture::getHasTransparency()
{
	return hasTransparency;
}
void ModelTexture::setHasTransparency(int newHasTransparency)
{
	hasTransparency = newHasTransparency;
}

int ModelTexture::getUsesFakeLighting()
{
	return useFakeLighting;
}
void ModelTexture::setUsesFakeLighting(int newUseFakeLighting)
{
	useFakeLighting = newUseFakeLighting;
}

float ModelTexture::getGlowAmount()
{
	return glowAmount;
}

void ModelTexture::setGlowAmount(float newGlowAmount)
{
	glowAmount = newGlowAmount;
}

void ModelTexture::setScrollX(float speedX)
{
	scrollX = speedX;
}

void ModelTexture::setScrollY(float speedY)
{
	scrollY = speedY;
}

float ModelTexture::getScrollX()
{
	return scrollX;
}

float ModelTexture::getScrollY()
{
	return scrollY;
}

void ModelTexture::deleteMe()
{
	Loader::deleteTexture(texID);
}
