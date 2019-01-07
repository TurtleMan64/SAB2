#include "collideableobject.h"

#include "../collision/collisionmodel.h"


CollideableObject::CollideableObject()
{

}

void CollideableObject::updateCMJustPosition()
{
	collideModelOriginal->transformModel(collideModelTransformed, getPosition());
}

void CollideableObject::updateCollisionModel()
{
	collideModelOriginal->transformModel(collideModelTransformed, getPosition(), -getRotY());
}

void CollideableObject::updateCollisionModelWithScale()
{
	collideModelOriginal->transformModelWithScale(collideModelTransformed, getPosition(), -getRotY(), getScale());
}

void CollideableObject::updateCollisionModelWithZ()
{
	collideModelOriginal->transformModel(collideModelTransformed, getPosition(), -getRotY(), getRotZ());
}

void CollideableObject::updateCMJustPosition(CollisionModel* cmBase, CollisionModel* cmToUpdate)
{
	cmBase->transformModel(cmToUpdate, getPosition());
}

void CollideableObject::updateCollisionModel(CollisionModel* cmBase, CollisionModel* cmToUpdate)
{
	cmBase->transformModel(cmToUpdate, getPosition(), -getRotY());
}

void CollideableObject::updateCollisionModelWithZ(CollisionModel* cmBase, CollisionModel* cmToUpdate)
{
	cmBase->transformModel(cmToUpdate, getPosition(), -getRotY(), getRotZ());
}
