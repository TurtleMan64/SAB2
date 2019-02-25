#ifndef PULLEY
#define PULLEY

#include <list>
#include "entity.h"
#include "collideableobject.h"

class TexturedModel;

class Pulley : public CollideableObject
{
private:
	static std::list<TexturedModel*> modelsHandle;
	static std::list<TexturedModel*> modelsRope;	
	static std::list<TexturedModel*> modelsTop;
	static CollisionModel* cmTop;

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