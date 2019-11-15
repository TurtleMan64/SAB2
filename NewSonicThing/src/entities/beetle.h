#ifndef SPINNER_H
#define SPINNER_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.h"


class Beetle : public Entity
{
private:
    static std::list<TexturedModel*> modelsBody;
    static std::list<TexturedModel*> modelsBlades;
    
    const float activationRange = 140.0f;
    const float hitRadius = 12.0f;
    const float bobHeight = 1.0f;
    const float bobPeriodScale = 2.0f;
    const float lookSpeed = 6.0f;

    Dummy* blades = nullptr;
    float bobTimer = 0.0f;
    Vector3f lookDir;

    //calculate new blades position
    void updateBlades(Vector3f* pos);

public:
    Beetle();
    Beetle(float x, float y, float z, std::list<Entity*>* entityListToAdd);

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
