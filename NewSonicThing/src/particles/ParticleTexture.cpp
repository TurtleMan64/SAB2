#include "particletexture.hpp"
#include <glad/glad.h>

ParticleTexture::ParticleTexture(GLuint textureId, int numberOfRows, float opacity, float glow)
{
    this->textureId = textureId;
    this->numberOfRows = numberOfRows;
    this->opacity = opacity;
    this->glow = glow;
}
