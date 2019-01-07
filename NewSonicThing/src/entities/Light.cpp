#include <glad/glad.h>

#include "../toolbox/vector.h"
#include "light.h"

Light::Light()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
}

Vector3f* Light::getPosition()
{
	return &position;
}
void Light::setPosition(Vector3f* newPosition)
{
	position.x = newPosition->x;
	position.y = newPosition->y;
	position.z = newPosition->z;
}
void Light::setPosition(float newX, float newY, float newZ)
{
	position.x = newX;
	position.y = newY;
	position.z = newZ;
}

Vector3f* Light::getColour()
{
	return &colour;
}
void Light::setColour(Vector3f* newColour)
{
	colour.x = newColour->x;
	colour.y = newColour->y;
	colour.z = newColour->z;
}
void Light::setColour(float newR, float newG, float newB)
{
	colour.x = newR;
	colour.y = newG;
	colour.z = newB;
}
