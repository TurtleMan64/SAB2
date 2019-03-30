#include <glad/glad.h>

#include "../models/models.h"
#include "../toolbox/vector.h"
#include "camera.h"
#include "../toolbox/input.h"
#include "../toolbox/maths.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../engineTester/main.h"

#include <cmath>


Camera::Camera()
{
	eye.set(0,20,0);
	target.set(1,20,0);
	up.set(0,1,0);
}

void Camera::mirrorForWater()
{
	Vector3f waterNormal(0, 1, 0);
	Vector3f view = target - eye;
	view = Maths::bounceVector(&view, &waterNormal, 1.0f);
	Vector3f newUp = Maths::bounceVector(&up, &waterNormal, 1.0f);
	newUp.neg();
	up = newUp;

	float yDiff = eye.y - Global::waterHeight;
	eye.y -= yDiff*2;
	target = eye+view;
}

void Camera::refresh()
{
	//float xDiff = position.x - target.x;
	//float zDiff = position.z - target.z;
	//float yDiff = position.y - target.y;
	//float hDist = sqrtf(xDiff*xDiff + zDiff*zDiff);

	//yaw   = -toDegrees(atan2f(-zDiff, xDiff))-90;
	//pitch =  toDegrees(atan2f(yDiff, hDist));

	//Vector3f off(
	//	-cosf(Maths::toRadians(yaw + 90))*((cosf(Maths::toRadians(pitch)))),
	//	 sinf(Maths::toRadians(pitch + 180)),
	//	-sinf(Maths::toRadians(yaw + 90))*((cosf(Maths::toRadians(pitch)))));
	//
	//fadePosition1.x = position.x + off.x*0.7f;
	//fadePosition1.y = position.y + off.y*0.7f;
	//fadePosition1.z = position.z + off.z*0.7f;
	//
	//fadePosition2.x = position.x + off.x*0.55f;
	//fadePosition2.y = position.y + off.y*0.55f;
	//fadePosition2.z = position.z + off.z*0.55f;
	//
	//if (position.y < 0)
	//{
	//	Vector3f partVel(0, 0, 0);
	//	new Particle(ParticleResources::textureInWater, &fadePosition1, &partVel, 0, 10, 0, 400, 0, true);
	//}

	Vector3f off;
	off = target-eye;

	off.normalize();
	off.scale(1.7f);
	fadePosition1.set(eye.x + off.x, eye.y + off.y, eye.z + off.z);

	off.normalize();
	off.scale(1.55f);
	fadePosition2.set(eye.x + off.x, eye.y + off.y, eye.z + off.z);

	extern float dt;
	vel = (eye-eyePrevious);
	vel.scale(1/dt);
	eyePrevious.set(&eye);
}

Vector3f* Camera::getFadePosition1()
{
	return &fadePosition1;
}

Vector3f* Camera::getFadePosition2()
{
	return &fadePosition2;
}

//void Camera::setTarget(float x, float y, float z)
//{
	//target.set(x, y, z);
//}

//void Camera::setTarget(Vector3f* newTarget)
//{
	//target.set(newTarget);
//}

void Camera::setViewMatrixValues(Vector3f* newEye, Vector3f* newTarget, Vector3f* newUp)
{
	eye.set(newEye);
	target.set(newTarget);
	up.set(newUp);
}
