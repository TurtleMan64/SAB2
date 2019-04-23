#ifndef WINDGUST_H
#define WINDGUST_H

#include <list>
#include "entity.h"

class WindGust : public Entity
{
private:
    float radius;
	float radiusSquared;

	float height;
	float power;

	//Functions for step() start here

	//Check if the player is within the hitbox of the wind gust
	bool checkForPlayerCollision();

public:
	WindGust(float x, float y, float z, float rotY, float rotZ, float radius, float height, float power);

	void step();
};
#endif