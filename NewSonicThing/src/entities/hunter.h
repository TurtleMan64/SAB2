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
    const float bulletSpeed = 120.0f;

    float shootTimer = 0.0f;
    Vector3f gunDirection;
    Vector3f gunTipPos;

    Dummy* gun = nullptr;
    Vector3f forward;
    Vector3f centerPos;

    void updateGun();

public:
    Hunter();
    Hunter(float x, float y, float z, float dx, float dy, float dz, std::list<Entity*>* entityListToAdd);

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
