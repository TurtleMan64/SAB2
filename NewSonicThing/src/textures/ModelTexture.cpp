#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <unordered_set>

#include "modeltexture.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/maths.h"

std::unordered_set<ModelTexture*> ModelTexture::animatedTextureReferences;

ModelTexture::ModelTexture()
{
	this->shineDamper = 20.0f;
	this->reflectivity = 0.0f;
	this->hasTransparency = false;
	this->useFakeLighting = false;
	this->glowAmount = 0;
    this->isAnimated = false;
    this->animatedProgress = 0.0f;
    this->animationSpeed = 0.0f;
    this->currentImageIndex = 0;
    this->mixingType = 1;
}

ModelTexture::ModelTexture(std::vector<GLuint>* texIDs)
{
    for (GLuint id : (*texIDs))
    {
        this->texIDs.push_back(id);
    }
	this->shineDamper = 20.0f;
	this->reflectivity = 0.0f;
	this->hasTransparency = false;
	this->useFakeLighting = false;
	this->glowAmount = 0;
    this->isAnimated = false;
    this->animatedProgress = 0.0f;
    this->animationSpeed = 0.0f;
    this->currentImageIndex = 0;
    this->mixingType = 1;
    this->scrollX = 0.0f;
	this->scrollY = 0.0f;

    if (this->texIDs.size() > 1)
    {
        this->isAnimated = true;
    }
}

ModelTexture::ModelTexture(ModelTexture* other)
{
    std::vector<GLuint>* otherIDs = other->getIDs();
    for (GLuint id : (*otherIDs))
    {
        this->texIDs.push_back(id);
    }
	this->shineDamper       = other->shineDamper;
	this->reflectivity      = other->reflectivity;
	this->hasTransparency   = other->hasTransparency;
	this->useFakeLighting   = other->useFakeLighting;
	this->glowAmount        = other->glowAmount;
    this->isAnimated        = other->isAnimated;
    this->animatedProgress  = other->animatedProgress;
    this->animationSpeed    = other->animationSpeed;
    this->currentImageIndex = other->currentImageIndex;
    this->mixingType        = other->mixingType;
    this->scrollX           = other->scrollX;
	this->scrollY           = other->scrollY;

    if (this->texIDs.size() > 1)
    {
        this->isAnimated = true;
    }
}

GLuint ModelTexture::getID()
{
	return texIDs[currentImageIndex];
}

GLuint ModelTexture::getID2()
{
	return texIDs[(currentImageIndex+1)%texIDs.size()];
}

bool ModelTexture::hasMultipleImages()
{
    return isAnimated;
}

void ModelTexture::deleteMe()
{
    for (GLuint id : texIDs)
    {
	    Loader::deleteTexture(id);
    }
    texIDs.clear();
    ModelTexture::animatedTextureReferences.erase(this);
}

float ModelTexture::mixFactor()
{
    switch (mixingType)
    {
        case 1:
            return 0.0f;

        case 2:
            return animatedProgress;

        case 3:
            return 0.5f*(sinf(Maths::PI*(animatedProgress - 0.5f)) + 1);
    }

    return 0.0f;
}

std::vector<GLuint>* ModelTexture::getIDs()
{
    return &texIDs;
}

void ModelTexture::addMeToAnimationsSetIfNeeded()
{
    if (isAnimated)
    {
        ModelTexture::animatedTextureReferences.insert(this);
    }
}

void ModelTexture::updateAnimations(float dt)
{
    for (ModelTexture* tex : ModelTexture::animatedTextureReferences)
    {
        tex->animatedProgress += tex->animationSpeed*dt;
        if (tex->animatedProgress >= 1.0f)
        {
            tex->currentImageIndex = (tex->currentImageIndex+1)%tex->texIDs.size();
            tex->animatedProgress-=1.0f;
        }
    }
}
