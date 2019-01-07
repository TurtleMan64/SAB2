#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>


class Keyframe
{
public:
	float time;
	float x;
	float y;
	float z;
	float xRot;
	float yRot;
	float zRot;
	float sRot;
	float scale;

	Keyframe(float time, float x, float y, float z,
		float xRot, float yRot, float zRot, float sRot, float scale);
};
#endif