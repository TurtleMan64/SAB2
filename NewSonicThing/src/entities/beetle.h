#ifndef SPINNER_H
#define SPINNER_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.h"


class Beetle : public Entity
{
private:
	static std::list<TexturedModel*> modelsBody;
	static std::list<TexturedModel*> modelsBlades;
	
	static float hitRadius;

	Dummy* blades = nullptr;

    //calculate new blades position
    void updateBlades();

public:
	Beetle();
	Beetle(float x, float y, float z, std::list<Entity*>* entityListToAdd);

	void step();

	void die();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();

	const bool canHomingAttackOn();

	const Vector3f getHomingCenter();

	const bool isEnemy();
};
#endif
