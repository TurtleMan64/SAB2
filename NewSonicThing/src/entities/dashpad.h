#ifndef DASHPAD_H
#define DASHPAD_H

class TexturedModel;

#include <list>
#include "entity.h"


class Dashpad : public Entity
{
private:
	static std::list<TexturedModel*> models;
	bool playerIsIn;
    float power;
	float controlLockTime;
    Vector3f forward;
    Vector3f up;

public:
	Dashpad();
	Dashpad(
		float x,     float y,     float z,
        float power, float controlLockTime,
		float rotX, float rotY, float rotZ);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
