#include "particletexture.h"
#include <glad/glad.h>

ParticleTexture::ParticleTexture(GLuint textureID, int numberOfRows, float opacity, float glow)
{
	this->textureID = textureID;
	this->numberOfRows = numberOfRows;
	this->opacity = opacity;
	this->glow = glow;
}

GLuint ParticleTexture::getTextureID()
{
	return textureID;
}

int ParticleTexture::getNumberOfRows()
{
	return numberOfRows;
}

float ParticleTexture::getOpacity()
{
	return opacity;
}

float ParticleTexture::getGlow()
{
	return glow;
}
