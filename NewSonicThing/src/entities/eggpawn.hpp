#ifndef EGGPAWN_H
#define EGGPAWN_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class EggPawn : public Entity
{
private:
    static std::vector<TexturedModel*> modelsStand;
    static std::vector<std::vector<TexturedModel*>> modelsWalk;
    
    static constexpr float centerHeight = 12.7f;
    static constexpr float collisionRadiusPawn = 24.0f;
    static constexpr float collisionRadiusWall = 10.0f;
    static constexpr float activationRange = 200.0f;
    static constexpr float hitRadius = 13.5f;
    static constexpr float lookSpeed = 1.5f;

    bool isWalking = false;
    float hitTimer = 0.0f;
    Vector3f center;
    Vector3f lookDir;

public:
    static std::vector<EggPawn*> pawns;

    EggPawn();
    EggPawn(float x, float y, float z, float dx, float dz);

    void step();

    void die();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;

    bool isEnemy() const;
};
#endif
