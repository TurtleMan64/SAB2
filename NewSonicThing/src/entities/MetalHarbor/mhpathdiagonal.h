#ifndef MH_PATHDIAGONAL
#define MH_PATHDIAGONAL

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;

class MH_PathDiagonal : public CollideableObject
{
private:
	static std::list<TexturedModel*> models;
	static CollisionModel* cmOriginal;

public:
	MH_PathDiagonal();
	MH_PathDiagonal(float x, float y, float z, float rotY);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif