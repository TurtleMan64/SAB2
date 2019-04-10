#ifndef WINDGUST_H
#define WINDGUST_H

#include <list>
#include "entity.h"

class WindGust : public Entity
{
private:
    float radius;
	float height;
	float power;

public:
    WindGust();
	WindGust(float x, float y, float z, float rotY, float rotZ, float radius, float height, float power);

	void step();
};
#endif