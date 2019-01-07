#ifndef BOOSTPAD_H
#define BOOSTPAD_H

class TexturedModel;
class Dummy;
class CollisionModel;

#include <list>
#include "entity.h"


class Boostpad : public Entity
{
private:
	static std::list<TexturedModel*> modelsPad;
	static std::list<TexturedModel*> modelsBolt;
	static CollisionModel* cmOriginal;

	CollisionModel* cmTransformed;

	Dummy* bolt;
	bool playerIsIn;

public:
	Boostpad();
	Boostpad(
		float x,     float y,     float z, 
		float normX, float normY, float normZ,
		float atX,   float atY,   float atZ);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif