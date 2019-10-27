#ifndef SRINGTRIPLE_H
#define SRINGTRIPLE_H

class TexturedModel;

#include <list>
#include <vector>
#include "entity.h"


class SpringTriple : public Entity
{
public:
	static std::list<TexturedModel*> models;

	float springPower;
	float cooldownTimer;
	float cooldownTimerMax;

	const float HITBOX_DISTANCE_FROM_ORIGIN_HORIZONTAL = 5.88928f;
	const float HITBOX_DISTANCE_FROM_ORIGIN_VERTICAL = 5.29734f;
	const float HITBOX_RADIUS = 10;
	const float HITBOX_HEIGHT = 5.3;
	const float SIDE_SPRING_OFFSET = 15;

    Vector3f dir; //direction that the spring is facing
    Vector3f hitCenter; //the center point of hit detection for the spring
	Vector3f hitRight; //the point to the right of the center for hit detection for the spring
	Vector3f hitLeft; //the point to the left of the center for hit detection for the spring

	SpringTriple();
	SpringTriple(float x, float y, float z, float dirX, float dirZ, float myPower, float cooldownMax);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();

	const bool canHomingAttackOn();

	const Vector3f getHomingCenter();

	inline bool collisionCheckCylinder(Vector3f collisionCenterPos, float hitboxRadius, float hitboxHeight);

	inline void doSpringBounce(Vector3f hitboxPoint);
};
#endif
