#ifndef ECORCA_H
#define ECORCA_H

class TexturedModel;

#include <list>
#include "../entity.h"

class EC_Orca : public Entity
{
private:
    static std::list<TexturedModel*> models;

    float initX = 0.0f;
    float initY = 0.0f;
    float initZ = 0.0f;

    float xVel = 0.0f;
    float yVel = 0.0f;
    float zVel = 0.0f;

    float seeTimer = 0.0f;

    bool inWaterPrevious = true;

public:
    EC_Orca();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
