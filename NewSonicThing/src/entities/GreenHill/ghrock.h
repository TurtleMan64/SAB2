#ifndef GHROCK_H
#define GHROCK_H

class TexturedModel;
class CollisionModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class GH_Rock : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	GH_Rock();
	GH_Rock(float x, float y, float z, float rotY, float scale);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
