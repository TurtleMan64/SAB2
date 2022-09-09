#ifndef FLAPPER_H
#define FLAPPER_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"


class Flapper : public Entity
{
private:
    static std::list<TexturedModel*> modelsRedBody;
    static std::list<TexturedModel*> modelsRedWingRight;
    static std::list<TexturedModel*> modelsRedWingLeft;
    static std::list<TexturedModel*> modelsGreenBody;
    static std::list<TexturedModel*> modelsGreenWingRight;
    static std::list<TexturedModel*> modelsGreenWingLeft;
    static std::list<TexturedModel*> modelsGun;
    
    static constexpr float activationRange = 220.0f;
    static constexpr float lookSpeed = 5.0f;
    static constexpr float hitRadius = 9.0f;
    static constexpr float timeUntilShoot = 2.0f;

    float bulletSpeed = 120.0f; //this changes based on distance to sonic

    float shootTimer = 0.0f;
    Vector3f gunDirection; //the visual direction that the gun is facing
    Vector3f gunTipPos; //the top of the gun barrel

    Vector3f bulletDirection; //the direction the bullet is going to go in
    Vector3f bulletTipPos; //where the bullet is going to spawn

    Dummy* gun = nullptr;
    Vector3f forward;

    void updateGun();

    Dummy* wingLeft = nullptr;
    Dummy* wingRight = nullptr;

    void updateWings();

    int type = 0;

public:
    Flapper();
    Flapper(float x, float y, float z, int type, float dx, float dz, std::list<Entity*>* entityListToAdd);

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
