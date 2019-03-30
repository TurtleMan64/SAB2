#ifndef GOALRING_H
#define GOALRING_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.h"

class GoalRing : public Entity
{
private:
	static std::list<TexturedModel*> modelsRing;
	static std::list<TexturedModel*> modelsText;

	Dummy* text;

public:
	GoalRing();
	GoalRing(float x, float y, float z);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
