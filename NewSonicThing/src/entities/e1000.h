#ifndef E1000_H
#define E1000_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.h"

class E1000 : public Entity
{
private:
    static std::list<TexturedModel*> modelsBody;
    static std::list<TexturedModel*> modelsGun;
    static std::list<TexturedModel*> modelsBullet;
    
    const float activationRange = 220.0f;
    const float lookSpeed = 5.0f;
    const float hitRadius = 4.5f;
    const float hitHeight = 20.0f;
    const float timeUntilShoot = 2.0f;

    float bullet1Speed = 120.0f; //this changes based on distance to sonic
    float bullet2Speed = 120.0f; //this changes based on distance to sonic

    float shootTimer = 0.0f;
    Vector3f gun1Direction; //the visual direction that the gun is facing
    Vector3f gun1TipPos; //the top of the gun barrel
    Vector3f gun2Direction; //the visual direction that the gun is facing
    Vector3f gun2TipPos; //the top of the gun barrel

    Vector3f bullet1Direction; //the direction the bullet is going to go in
    Vector3f bullet1TipPos; //where the bullet is going to spawn
    Vector3f bullet2Direction; //the direction the bullet is going to go in
    Vector3f bullet2TipPos; //where the bullet is going to spawn

    Dummy* gun1 = nullptr;
    Dummy* gun2 = nullptr;
    Vector3f forward;
    Vector3f centerPos;

    void updateGun1();
    void updateGun2();

public:
    E1000();
    E1000(float x, float y, float z, float dx, float dz, std::list<Entity*>* entityListToAdd);

    void step();

    void die();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    bool canHomingAttackOn() const;

    Vector3f getHomingCenter() const;

    bool isEnemy() const;
};
#endif
