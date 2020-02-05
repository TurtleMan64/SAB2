#ifndef HUNTER_H
#define HUNTER_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.h"


class Hunter : public Entity
{
private:
    static std::list<TexturedModel*> modelsBody;
    static std::list<TexturedModel*> modelsGun;
    static std::list<TexturedModel*> modelsBullet;
    
    const float activationRange = 140.0f;
    const float lookSpeed = 5.0f;
    const float hitRadius = 7.2f;
    const float hitHeight = 27.0f;
    const float timeUntilShoot = 2.0f;

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
    Hunter();
    Hunter(float x, float y, float z, float dx, float dz, std::list<Entity*>* entityListToAdd);

    void step();

    void die();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    const bool canHomingAttackOn();

    const Vector3f getHomingCenter();

    const bool isEnemy();
};
#endif
