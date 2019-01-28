#ifndef MH_TANK
#define MH_TANK

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class MH_Tank : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	MH_Tank();
	MH_Tank(float x, float y, float z);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif