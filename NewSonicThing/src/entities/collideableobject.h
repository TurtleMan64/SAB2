#ifndef COLLIDEABLEOBJECT_H
#define COLLIDEABLEOBJECT_H

class CollisionModel;

#include "entity.h"

class CollideableObject : public Entity
{
protected:
    CollisionModel* collideModelOriginal;
    CollisionModel* collideModelTransformed;

public:
    CollideableObject();

    void updateCMJustPosition(CollisionModel* cmBase, CollisionModel* cmToUpdate);

    void updateCollisionModel(CollisionModel* cmBase, CollisionModel* cmToUpdate);

    void updateCollisionModelWithZ(CollisionModel* cmBase, CollisionModel* cmToUpdate);

    void updateCMJustPosition();

    void updateCollisionModel();

    void updateCollisionModelWithScale();

    void updateCollisionModelWithZ();

    void updateCollisionModelXYZ();
};
#endif