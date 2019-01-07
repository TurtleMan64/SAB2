#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "../toolbox/maths.h"
#include "../engineTester/main.h"
#include "../collision/collisionmodel.h"

#include <list>
#include <iostream>
#include <string>

Entity::Entity()
{
	this->position.x = 0;
	this->position.y = 0;
	this->position.z = 0;
	this->rotX = 0;
	this->rotY = 0;
	this->rotZ = 0; 
	this->rotRoll = 0;
	this->scale = 1;
	this->visible = true;
	this->baseColour.set(1,1,1);
}

Entity::Entity(Vector3f* position, float rotX, float rotY, float rotZ, float scale)
{
	this->position.x = position->x;
	this->position.y = position->y;
	this->position.z = position->z;
	this->rotX = rotX;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->rotRoll = 0;
	this->scale = scale;
	this->visible = true;
	this->baseColour.set(1,1,1);
}

Entity::~Entity()
{

}

void Entity::step()
{

}

bool Entity::isVehicle()
{
	return false;
}

bool Entity::isEmeraldPiece()
{
	return false;
}

bool Entity::isGoalTrigger()
{
	return false;
}

bool Entity::isEnemy()
{
	return false;
}

void Entity::die()
{
	
}

void Entity::increasePosition(float dx, float dy, float dz)
{
	position.x += dx;
	position.y += dy;
	position.z += dz;
}

void Entity::increaseRotation(float dx, float dy, float dz)
{
	rotX += dx;
	rotY += dy;
	rotZ += dz;
}

std::list<TexturedModel*>* Entity::getModels()
{
	return nullptr;
}

Vector3f* Entity::getPosition()
{
	return &position;
}
void Entity::setPosition(Vector3f* newPos)
{
	this->position.x = newPos->x;
	this->position.y = newPos->y;
	this->position.z = newPos->z;
}
void Entity::setPosition(float newX, float newY, float newZ)
{
	this->position.x = newX;
	this->position.y = newY;
	this->position.z = newZ;
}

Vector3f* Entity::getBaseColour()
{
	return &baseColour;
}

void Entity::setBaseColour(float red, float green, float blue)
{
	baseColour.set(red, green, blue);
}

float Entity::getRotX()
{
	return rotX;
}
void Entity::setRotX(float newRotX)
{
	rotX = newRotX;
}

float Entity::getRotY()
{
	return rotY;
}
void Entity::setRotY(float newRotY)
{
	rotY = newRotY;
}

float Entity::getRotZ()
{
	return rotZ;
}
void Entity::setRotZ(float newRotZ)
{
	rotZ = newRotZ;
}

float Entity::getRotSpin()
{
	return rotRoll;
}
void Entity::setRotSpin(float newRotSpin)
{
	rotRoll = newRotSpin;
}

float Entity::getScale()
{
	return scale;
}
void Entity::setScale(float newScale)
{
	scale = newScale;
}

bool Entity::getVisible()
{
	return visible;
}
void Entity::setVisible(bool newVisible)
{
	visible = newVisible;
}

float Entity::getX()
{
	return position.x;
}

float Entity::getY()
{
	return position.y;
}

float Entity::getZ()
{
	return position.z;
}

void Entity::setX(float newX)
{
	position.x = newX;
}

void Entity::setY(float newY)
{
	position.y = newY;
}

void Entity::setZ(float newZ)
{
	position.z = newZ;
}

void Entity::updateTransformationMatrix()
{
	Maths::createTransformationMatrix(&transformationMatrix, &position, rotX, rotY, rotZ, rotRoll, scale);
}

void Entity::updateTransformationMatrix(float scaleX, float scaleY, float scaleZ)
{
	Maths::createTransformationMatrix(&transformationMatrix, &position, rotX, rotY, rotZ, rotRoll, scaleX, scaleY, scaleZ);
}

void Entity::updateTransformationMatrixSADX()
{
	Maths::createTransformationMatrixSADX(&transformationMatrix, &position, rotX, rotY, rotZ, scale);
}

Matrix4f* Entity::getTransformationMatrix()
{
	return &transformationMatrix;
}

std::string Entity::getName()
{
	return "";
}

bool Entity::canLightdashOn()
{
	return false;
}

bool Entity::canHomingAttackOn()
{
	return false;
}

Vector3f Entity::getHomingCenter()
{
	return Vector3f(0,0,0);
}

bool Entity::isPoint()
{
	return false;
}

void Entity::deleteModels(std::list<TexturedModel*>* modelsToDelete)
{
	for (auto model : (*modelsToDelete))
	{
		model->deleteMe();
		delete model;
		INCR_DEL
	}
	modelsToDelete->clear();
}

void Entity::deleteCollisionModel(CollisionModel** colModelToDelete)
{
	if ((*colModelToDelete) != nullptr)
	{
		(*colModelToDelete)->deleteMe();
		delete (*colModelToDelete);
		INCR_DEL
		(*colModelToDelete) = nullptr;
	}
}