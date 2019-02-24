#ifndef PULLEY
#define PULLEY

#include <list>
#include "entity.h"

class TexturedModel;

class Pulley : public Entity
{
private:
	static std::list<TexturedModel*> models;

    float ropeYAxisScale;

public:
	Pulley();
    Pulley(float x, float y, float z);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif