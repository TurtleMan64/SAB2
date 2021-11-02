#ifndef KIKI_H
#define KIKI_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"

class Kiki : public Entity
{
private:
    static std::list<TexturedModel*> models;
    static std::list<TexturedModel*> modelsArm;
    
    const float activationRange = 140.0f;
    const float lookSpeed = 2.0f;
    const float timeUntilThrow = 5.0f;

    float throwTimer = 0.0f;

    Dummy* arm = nullptr;

    void updateArm();

    Vector3f forward;
    Vector3f centerPos;

public:
    Kiki();
    Kiki(
        float x, float y, float z,
        float dx, float dz,
        std::list<Entity*>* entityListToAdd);

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
