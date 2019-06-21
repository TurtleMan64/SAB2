#ifndef PARTICLE_H
#define PARTICLE_H

class ParticleTexture;
class Camera;

#include "../toolbox/vector.h"

class Particle
{
private:
	Vector3f position;
	Vector3f* positionRef;
	Vector3f velocity;
	float gravityEffect;
	float lifeLength;
	bool onlyRendersOnce;
	float rotation;
	float scaleX;
	float scaleXChange;
	float scaleY;
	float scaleYChange;
	float distance;
	ParticleTexture* texture;
	Vector2f texOffset1;
	Vector2f texOffset2;
	float blend;
	float elapsedTime = 0;
    float opacity;

	void updateTextureCoordInfo();

	void setTextureOffset(Vector2f* offset, int index);

public:
	Particle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce);

    Particle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce);

	Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce);

	Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
		float lifeLength, float rotation, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce);

	Particle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
		float lifeLength, float rotation, float scaleX, float scaleXChange, float scaleY, float scaleYChange, 
		bool posIsRef, bool onlyRendersOnce);

	float getDistance();

	ParticleTexture* getTexture();

	Vector3f* getPosition();

	float getRotation();

	float getScaleX();

	float getScaleY();

    float getOpacity();

	Vector2f* getTexOffset1();

	Vector2f* getTexOffset2();

	float getBlend();

	bool update(Camera* camera);
};
#endif
