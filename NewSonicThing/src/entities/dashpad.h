#ifndef DASHPAD_H
#define DASHPAD_H

class TexturedModel;

#include <list>
#include "entity.h"


class Dashpad : public Entity
{
private:
	static std::list<TexturedModel*> models;

	float power;
	float hitRadius;
	float camYawTarget;
	float cooldownTimer;
	float cooldownTimerMax;

public:
	Dashpad();
	Dashpad(float x, float y, float z, float rotX, float rotY, float rotZ, float myPower, float myCamYawTarget, float time);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif