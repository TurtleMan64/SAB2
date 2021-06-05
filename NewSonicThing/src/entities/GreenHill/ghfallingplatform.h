#ifndef GHFALLINGPLATFORM_H
#define GHFALLINGPLATFORM_H

class TexturedModel;
class CollisionModel;

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class GH_FallingPlatform : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;
	float timer = 0;

public:
	GH_FallingPlatform();
	GH_FallingPlatform(float x, float y, float z, float rotY, float scale);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
