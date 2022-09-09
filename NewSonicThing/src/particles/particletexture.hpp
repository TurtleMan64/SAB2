#ifndef PARTICLETEXTURE_H
#define PARTICLETEXTURE_H

#include <glad/glad.h>

class ParticleTexture
{
public:
    GLuint textureId;
    int numberOfRows;
    float opacity;
    float glow;

    ParticleTexture(GLuint textureId, int numberOfRows, float opacity, float glow);
};
#endif
