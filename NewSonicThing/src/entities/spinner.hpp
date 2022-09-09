#ifndef SPINNER_H
#define SPINNER_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class Spinner : public Entity
{
private:
    static std::list<TexturedModel*> modelsBody;
    static std::list<TexturedModel*> modelsBlades;
    
    constexpr static float activationRange = 140.0f;
    constexpr static float hitRadius = 12.0f;
    constexpr static float bobHeight = 1.0f;
    constexpr static float bobPeriodScale = 2.0f;
    constexpr static float lookSpeed = 6.0f;

    Dummy* blades = nullptr;
    float bobTimer = 0.0f;
    Vector3f lookDir;

    //calculate new blades position
    void updateBlades(Vector3f* pos);

public:
    Spinner();
    Spinner(float x, float y, float z, std::list<Entity*>* entityListToAdd);

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
