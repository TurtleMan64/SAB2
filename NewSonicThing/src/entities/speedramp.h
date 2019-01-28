#ifndef SPEEDRAMP_H
#define SPEEDRAMP_H

class TexturedModel;

#include <list>
#include "entity.h"
#include "collideableobject.h"

class SpeedRamp : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

	float power;
	int inputLockDuration;

public:
	SpeedRamp();
	SpeedRamp(float x, float y, float z, float rotY, float rotZ, float myPower, int inputLockDuration);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif