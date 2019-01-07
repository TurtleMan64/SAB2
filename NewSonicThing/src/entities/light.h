#ifndef LIGHT_H
#define LIGHT_H

#include "../toolbox/vector.h"


class Light
{
private:
	Vector3f position;
	Vector3f colour;

public:
	Light();

	Vector3f* getPosition();
	void setPosition(Vector3f*);
	void setPosition(float newX, float newY, float newZ);

	Vector3f* getColour();
	void setColour(Vector3f*);
	void setColour(float newR, float newG, float newB);
};
#endif