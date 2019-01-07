#ifndef CAMERA_H
#define CAMERA_H

#include "../toolbox/vector.h"

class Camera
{
public:
	Vector3f fadePosition1;
	Vector3f fadePosition2;

	Vector3f eye;
	Vector3f target;
	Vector3f up;

public:
	Camera();

	void refresh();

	Vector3f calcVelocity();

	Vector3f* getFadePosition1();

	Vector3f* getFadePosition2();

	void setViewMatrixValues(Vector3f* newEye, Vector3f* newTarget, Vector3f* newUp);
};
#endif