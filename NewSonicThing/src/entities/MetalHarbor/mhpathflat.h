#ifndef MH_PATHFLAT
#define MH_PATHFLAT

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class MH_PathFlat : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	MH_PathFlat();
	MH_PathFlat(float x, float y, float z);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif