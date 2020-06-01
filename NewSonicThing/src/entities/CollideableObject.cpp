#include "collideableobject.h"

#include "../collision/collisionmodel.h"


CollideableObject::CollideableObject()
{

}

void CollideableObject::updateCMJustPosition()
{
    collideModelOriginal->transformModel(collideModelTransformed, &position);
}

void CollideableObject::updateCollisionModel()
{
    collideModelOriginal->transformModel(collideModelTransformed, &position, rotY);
}

void CollideableObject::updateCollisionModelWithScale()
{
    collideModelOriginal->transformModelWithScale(collideModelTransformed, &position, rotY, scale);
}

void CollideableObject::updateCollisionModelWithZ()
{
    collideModelOriginal->transformModel(collideModelTransformed, &position, rotY, rotZ);
}

void CollideableObject::updateCollisionModelXYZ()
{
    collideModelOriginal->transformModel(collideModelTransformed, &position, rotX, rotY, rotZ);
}

void CollideableObject::updateCMJustPosition(CollisionModel* cmBase, CollisionModel* cmToUpdate)
{
    cmBase->transformModel(cmToUpdate, &position);
}

void CollideableObject::updateCollisionModel(CollisionModel* cmBase, CollisionModel* cmToUpdate)
{
    cmBase->transformModel(cmToUpdate, &position, rotY);
}

void CollideableObject::updateCollisionModelWithZ(CollisionModel* cmBase, CollisionModel* cmToUpdate)
{
    cmBase->transformModel(cmToUpdate, &position, rotY, rotZ);
}
