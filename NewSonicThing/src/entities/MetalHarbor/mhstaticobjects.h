#ifndef MH_STATICOBJECTS
#define MH_STATICOBJECTS

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class MH_StaticObjects : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	MH_StaticObjects();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif