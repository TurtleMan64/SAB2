#ifndef ITEMCAPSULE_H
#define ITEMCAPSULE_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"


class ItemCapsule : public Entity
{
private:
    static std::vector<TexturedModel*> modelsAnchoredBase;
    static std::vector<TexturedModel*> modelsAnchoredGlass;
    static std::vector<TexturedModel*> modelsFloatingBase;
    static std::vector<TexturedModel*> modelsFloatingGlass;

    static std::vector<TexturedModel*> modelsItem1Up;
    static std::vector<TexturedModel*> modelsItemBomb;
    static std::vector<TexturedModel*> modelsItemInvincible;
    static std::vector<TexturedModel*> modelsItemBarrierG;
    static std::vector<TexturedModel*> modelsItemRing5;
    static std::vector<TexturedModel*> modelsItemRing10;
    static std::vector<TexturedModel*> modelsItemRing20;
    static std::vector<TexturedModel*> modelsItemSpeedUp;
    static std::vector<TexturedModel*> modelsItemBarrierB;

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

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;
};
#endif
