#ifndef SPIKEBALL_H
#define SPIKEBALL_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class Spikeball : public Entity
{
private:
	static std::vector<TexturedModel*> models;

    Vector3f center;

	float radius;
	float angVel;

public:
	Spikeball();
	Spikeball(float x, float y, float z, float radius, float angVel);

	void step();

	std::vector<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
