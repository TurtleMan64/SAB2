#ifndef LIMB_H
#define LIMB_H

class Vector3f;
class TexturedModel;
class Body;

#include <list>
#include "../entities/entity.h"
#include "animation.h"


class Limb : public Entity
{
private:
	Body* body;
	Limb* limb;

	//We do not store our own models. It is up to 
	// whoever created us to create the models and keep
	// them allocated as long as we are alive.
	std::list<TexturedModel*>* myModels;

public:
	int animationIndex;
	float pivotX;
	float pivotY;
	float pivotZ;

	std::vector<Animation>* animations;

	Limb(std::list<TexturedModel*>* models, float pivotX, float pivotY, float pivotZ, Body* body, Limb* limb);

	void step();

	void update(float time);

	std::list<TexturedModel*>* getModels();

	//std::string getName();
};
#endif