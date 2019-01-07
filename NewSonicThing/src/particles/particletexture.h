#ifndef PARTICLETEXTURE_H
#define PARTICLETEXTURE_H

#include <glad/glad.h>

class ParticleTexture
{
private:
	GLuint textureID;
	int numberOfRows;
	float opacity;
	float glow;

public:
	ParticleTexture(GLuint textureID, int numberOfRows, float opacity, float glow);

	GLuint getTextureID();

	int getNumberOfRows();

	float getOpacity();

	float getGlow();
};
#endif