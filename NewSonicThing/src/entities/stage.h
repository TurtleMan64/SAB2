#ifndef STAGE_H
#define STAGE_H

class TexturedModel;

#include <list>
#include "entity.h"
#include "../toolbox/vector.h"
#include <string>


class Stage : public Entity
{
private:
	static std::list<TexturedModel*> models;

public:
	Vector3f finishPlayerPosition;
	float finishPlayerRotY;
	float finishCameraPitch;

	Stage();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadModels(char* folder, char* name);

	static void deleteModels();

	std::string getName();
};
#endif