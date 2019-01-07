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
	Entity(Vector3f*, float, float, float, float);
	virtual ~Entity();

	virtual void step();

	virtual bool isVehicle();

	virtual bool isEmeraldPiece();

	virtual bool isGoalTrigger();

	virtual bool isEnemy();

	void increasePosition(float, float, float);

	void increaseRotation(float, float, float);

	virtual std::list<TexturedModel*>* getModels();

	Vector3f* getPosition();
	void setPosition(Vector3f*);
	void setPosition(float newX, float newY, float newZ);

	Vector3f* getBaseColour();
	void setBaseColour(float red, float green, float blue);

	float getRotX();
	void setRotX(float);

	float getRotY();
	void setRotY(float);

	float getRotZ();
	void setRotZ(float);

	float getRotSpin();
	void setRotSpin(float);

	float getScale();
	void setScale(float);

	bool getVisible();
	void setVisible(bool);

	float getX();

	float getY();

	float getZ();

	void setX(float newX);

	void setY(float newY);

	void setZ(float newZ);

	Matrix4f* getTransformationMatrix();

	void updateTransformationMatrix();

	void updateTransformationMatrix(float scaleX, float scaleY, float scaleZ);

	void updateTransformationMatrixSADX();

	virtual std::string getName();

	virtual bool canLightdashOn();

	virtual bool canHomingAttackOn();

	virtual Vector3f getHomingCenter();
	
	virtual bool isPoint();

	virtual void die();
};
#endif