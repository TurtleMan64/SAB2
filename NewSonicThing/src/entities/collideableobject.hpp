#ifndef COLLIdEABLEOBJECT_H
#define COLLIdEABLEOBJECT_H

class CollisionModel;

#include "entity.hpp"

class CollideableObject : public Entity
{
protected:
    CollisionModel* collideModelOriginal = nullptr;
    CollisionModel* collideModelTransformed = nullptr;

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
