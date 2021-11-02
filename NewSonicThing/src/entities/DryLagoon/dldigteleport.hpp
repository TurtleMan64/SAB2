#ifndef DLDIGTELEPORT_H
#define DLDIGTELEPORT_H

class TexturedModel;
class CollisionModel;

#include <list>

#include "../entity.hpp"
#include "../collideableobject.hpp"
#include "../../toolbox/vector.hpp"

class DL_DigTeleport : public CollideableObject
{
private:
    static std::list<TexturedModel*> models;
    static CollisionModel* cmOriginal;

    float prevDiggTimer = 0.0f;
    float timeUntilWarp = -1.0f;
    bool doTeleport = false;
    Vector3f teleportLocation;

public:
    DL_DigTeleport(
        float x, float y, float z, 
        float yRot,
        float xTarget, float yTarget, float zTarget,
        float r, float g, float b);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
