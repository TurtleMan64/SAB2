#ifndef ENTITIES_H
#define ENTITIES_H

class TexturedModel;
class CollisionModel;

#include <list>
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include <string>


class Entity
{
protected:
	Vector3f position;
	float rotX, rotY, rotZ, rotRoll;
	float scale;
	bool visible;
	Vector3f baseColour;
	Matrix4f transformationMatrix;

	static void deleteModels(std::list<TexturedModel*>* modelsToDelete);
	static void deleteCollisionModel(CollisionModel** colModelToDelete);

public:
	Entity();
	Entity(Vector3f* initialPosition, float rotX, float rotY, float rotZ, float scale);
	virtual ~Entity();

	virtual void step();

	virtual const bool isVehicle();

	virtual const bool isEmeraldPiece();

	virtual const bool isGoalTrigger();

	virtual const bool isEnemy();

	void increasePosition(float dx, float dy, float dz);

	void increaseRotation(float dx, float dy, float dz);

	virtual std::list<TexturedModel*>* getModels();

	Vector3f* getPosition();
	void setPosition(Vector3f* newPosition);
	void setPosition(float newX, float newY, float newZ);

	Vector3f* getBaseColour();
	void setBaseColour(float red, float green, float blue);

	const float getRotX();
	void setRotX(float newRotX);

	const float getRotY();
	void setRotY(float newRotY);

	const float getRotZ();
	void setRotZ(float newRotZ);

	const float getRotSpin();
	void setRotSpin(float newRotSpin);

	const float getScale();
	void setScale(float newScale);

	const bool getVisible();
	void setVisible(bool newVisible);

	const float getX();

	const float getY();

	const float getZ();

	void setX(float newX);

	void setY(float newY);

	void setZ(float newZ);

	Matrix4f* getTransformationMatrix();

	void updateTransformationMatrix();

	void updateTransformationMatrix(float scaleX, float scaleY, float scaleZ);

	void updateTransformationMatrixSADX();

	virtual const bool canLightdashOn();

	virtual const bool canHomingAttackOn();

	virtual const Vector3f getHomingCenter();
	
	virtual const bool isPoint();

	virtual void die();
};
#endif
