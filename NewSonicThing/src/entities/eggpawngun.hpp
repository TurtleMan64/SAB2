#ifndef EGGPAWNGUN_H
#define EGGPAWNGUN_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"

class EggPawnGun : public Entity
{
private:
    static std::vector<TexturedModel*> modelsBody;
    static std::vector<TexturedModel*> modelsGun;
    
    static constexpr float activationRange = 140.0f;
    static constexpr float lookSpeed = 5.0f;
    static constexpr float hitRadius = 8.5f;
    static constexpr float hitHeight = 27.0f;
    static constexpr float timeUntilShoot = 2.0f;

    float bulletSpeed = 120.0f; //this changes based on distance to sonic

    float shootTimer = 0.0f;
    Vector3f gunDirection; //the visual direction that the gun is facing
    Vector3f gunTipPos; //the top of the gun barrel

    Vector3f bulletDirection; //the direction the bullet is going to go in
    Vector3f bulletTipPos; //where the bullet is going to spawn

    Dummy* gun = nullptr;
    Vector3f forward;
    Vector3f centerPos;

    void updateGun();

public:
    EggPawnGun();
    EggPawnGun(float x, float y, float z, float dx, float dz, std::list<Entity*>* entityListToAdd);

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
