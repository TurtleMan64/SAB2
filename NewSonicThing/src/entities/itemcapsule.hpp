#ifndef ITEMCAPSULE_H
#define ITEMCAPSULE_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"


class ItemCapsule : public Entity
{
private:
    static std::list<TexturedModel*> modelsAnchoredBase;
    static std::list<TexturedModel*> modelsAnchoredGlass;
    static std::list<TexturedModel*> modelsFloatingBase;
    static std::list<TexturedModel*> modelsFloatingGlass;

    static std::list<TexturedModel*> modelsItem1Up;
    static std::list<TexturedModel*> modelsItemBomb;
    static std::list<TexturedModel*> modelsItemInvincible;
    static std::list<TexturedModel*> modelsItemBarrierG;
    static std::list<TexturedModel*> modelsItemRing5;
    static std::list<TexturedModel*> modelsItemRing10;
    static std::list<TexturedModel*> modelsItemRing20;
    static std::list<TexturedModel*> modelsItemSpeedUp;
    static std::list<TexturedModel*> modelsItemBarrierB;

    static constexpr float BOMB_RADIUS = 400.0f;

    Vector3f centerPos;
    Vector3f upDir;

    int itemType = 0;
    int boxType = 0;
    Dummy* entityGlass = nullptr;
    Dummy* entityItem = nullptr;

public:
    ItemCapsule();
    ItemCapsule(float x, float y, float z, float upX, float upY, float upZ, int itemType, int boxType, std::list<Entity*>* entityListToAdd);

    void step();

    void die();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;
};
#endif
