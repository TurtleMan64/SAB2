#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "../toolbox/maths.hpp"
#include "../engineTester/main.hpp"
#include "../collision/collisionmodel.hpp"
#include "../models/models.hpp"

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
    this->baseColor.set(1,1,1);
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
    this->baseColor.set(1,1,1);
}

Entity::~Entity()
{

}

void Entity::step()
{

}

bool Entity::isVehicle() const
{
    return false;
}

bool Entity::isEmeraldPiece() const
{
    return false;
}

bool Entity::isGoalTrigger() const
{
    return false;
}

bool Entity::isEnemy() const
{
    return false;
}

bool Entity::isPoint() const
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

Vector3f* Entity::getBaseColor()
{
    return &baseColor;
}

void Entity::setBaseColor(float red, float green, float blue)
{
    baseColor.set(red, green, blue);
}

void Entity::setModelsRenderOrder(std::list<TexturedModel*>* models, char newOrder)
{
    for (TexturedModel* model : (*models))
    {
        model->renderOrder = newOrder;
    }
}

void Entity::updateTransformationMatrix()
{
    Maths::createTransformationMatrix(&transformationMatrix, &position, rotX, rotY, rotZ, rotRoll, scale);
}

void Entity::updateTransformationMatrixYXZY()
{
    Maths::createTransformationMatrixYXZY(&transformationMatrix, &position, rotX, rotY, rotZ, rotRoll, scale);
}

void Entity::updateTransformationMatrix(float scaleX, float scaleY, float scaleZ)
{
    Maths::createTransformationMatrix(&transformationMatrix, &position, rotX, rotY, rotZ, rotRoll, scaleX, scaleY, scaleZ);
}

void Entity::updateTransformationMatrixYXZ()
{
    Maths::createTransformationMatrixYXZ(&transformationMatrix, &position, rotX, rotY, rotZ, scale);
}

bool Entity::canLightdashOn() const
{
    return false;
}

bool Entity::canHomingAttackOn() const
{
    return false;
}

Vector3f Entity::getHomingCenter() const
{
    return Vector3f(0,0,0);
}

void Entity::deleteModels(std::list<TexturedModel*>* modelsToDelete)
{
    for (auto it = modelsToDelete->begin(); it != modelsToDelete->end(); it++)
    {
        TexturedModel* model = it._Ptr->_Myval;
        model->deleteMe();
        delete model; INCR_DEL("TexturedModel");
    }
    modelsToDelete->clear();
}

void Entity::deleteCollisionModel(CollisionModel** colModelToDelete)
{
    if ((*colModelToDelete) != nullptr)
    {
        (*colModelToDelete)->deleteMe();
        delete (*colModelToDelete); INCR_DEL("CollisionModel");
        (*colModelToDelete) = nullptr;
    }
}
