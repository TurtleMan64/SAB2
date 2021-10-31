#include "particletexture.h"
#include <glad/glad.h>

ParticleTexture::ParticleTexture(GLuint textureId, int numberOfRows, float opacity, float glow)
{
    this->textureId = textureId;
    this->numberOfRows = numberOfRows;
    this->opacity = opacity;
    this->glow = glow;
}

GLuint ParticleTexture::getTextureId()
{
    return textureId;
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
