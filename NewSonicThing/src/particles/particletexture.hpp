#ifndef PARTICLETEXTURE_H
#define PARTICLETEXTURE_H

#include <glad/glad.h>

class ParticleTexture
{
private:
    GLuint textureId;
    int numberOfRows;
    float opacity;
    float glow;

public:
    ParticleTexture(GLuint textureId, int numberOfRows, float opacity, float glow);

    GLuint getTextureId();

    int getNumberOfRows();

    float getOpacity();

    float getGlow();
};
#endif
