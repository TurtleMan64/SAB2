#ifndef MOTOBUG_H
#define MOTOBUG_H

class TexturedModel;
class Triangle3D;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class MotoBug : public Entity
{
private:
	static std::list<TexturedModel*> models;

    constexpr static float activationRange = 86.0f; //range where they can see sonic
    constexpr static float moveRange = 600.0f; //range where they start wandering around
    constexpr static float lookSpeed = 6.0f; //how fast they turn towards sonic

    constexpr static float hitboxRadius = 12.2f;
    constexpr static float hitboxHeight = 20.4f;

    Vector3f lookDir;

	Triangle3D* triCol = nullptr;
	float xVel = 0;
	float yVel = 0;
	float zVel = 0;
	float furyTimer = 0;
	constexpr static float furyTimerMax = 1.666f;
	bool inAir = true;
	float clock = 0;
	float animIndex = 0;

	void applyFriction(float frictionToApply);

    void moveMeSlow();

    void moveMe();

    bool playerIsInHitbox();

public:
	MotoBug();
	MotoBug(float x, float y, float z);

	void step();

	void die();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();

	bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;

    bool isEnemy() const;
};
#endif
