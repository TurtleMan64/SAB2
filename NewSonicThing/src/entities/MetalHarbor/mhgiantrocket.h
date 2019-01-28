#ifndef MH_GIANTROCKET
#define MH_GIANTROCKET

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class MH_GiantRocket : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	MH_GiantRocket();
	MH_GiantRocket(float x, float y, float z);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif