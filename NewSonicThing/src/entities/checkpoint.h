#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <list>
#include "entity.h"

class TexturedModel;

class Checkpoint : public Entity
{
private:
	static std::list<TexturedModel*> models;

	float scaleX;
	float scaleY;
	float scaleZ;

	float sinT;
	float cosT;

public:
	int ID;

	Checkpoint();
	Checkpoint(
		float rotationY, float x, float y, float z,
		float scaleX, float scaleY, float scaleZ,
		int number);

	void step();

	bool isPointInsideMe(Vector3f* test);

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif