#ifndef MH_ROCKETBASE
#define MH_ROCKETBASE

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class MH_RocketBase : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	MH_RocketBase();
	MH_RocketBase(float x, float y, float z);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif