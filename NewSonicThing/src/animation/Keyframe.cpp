#include "keyframe.h"

Keyframe::Keyframe(float time, float x, float y, float z,
	float xRot, float yRot, float zRot, float sRot, float scale)
{
	this->time = time;
	this->x = x;
	this->y = y;
	this->z = z;
	this->xRot = xRot;
	this->yRot = yRot;
	this->zRot = zRot;
	this->sRot = sRot;
	this->scale = scale;
}