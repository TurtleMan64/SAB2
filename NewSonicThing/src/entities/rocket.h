#ifndef ROCKET_H
#define ROCKET_H

class TexturedModel;
class Body;
class Source;

#include <list>
#include "./entity.h"
#include "./collideableobject.h"


class Rocket : public CollideableObject
{
private:
	static std::list<TexturedModel*> modelsRocket;
	static std::list<TexturedModel*> modelsBase;
	static CollisionModel* cmBase;

	Body* base;

	Vector3f pointPos1;
	Vector3f pointPos2;

	const float speed = 15;
	bool isMoving;
	bool canMove;

	const float radius = 5;
	const float height = 15;

	Vector3f pointDifference;
	Vector3f pointDifferenceNormalized;
	float pointLength;

	float timeOffset;
	float startupTimer;

	bool rocketAppearSoundPlayed;
	Source* rocketSource;

public:
	Rocket();
	Rocket(int point1ID, int point2ID);

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif