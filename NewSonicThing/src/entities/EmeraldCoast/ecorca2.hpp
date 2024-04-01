#ifndef ECORCA2_H
#define ECORCA2_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class EC_Orca2 : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    float initX = 0.0f;
    float initY = 0.0f;
    float initZ = 0.0f;

    float xVel = 0.0f;
    float yVel = 0.0f;
    float zVel = 0.0f;

    float seeTimer = 0.0f;

    bool inWaterPrevious = true;

public:
    EC_Orca2();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
