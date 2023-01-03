#ifndef DASHRING_H
#define DASHRING_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class DashRing : public Entity
{
private:
    static std::vector<TexturedModel*> modelsDash;
    static std::vector<TexturedModel*> modelsRainbow;

    bool isRainbow = false;
    bool playerIsIn = false;
    float ringRadius = 1.0f;
    float power = 100.0f;
    float controlLockTime = 1.0f;
    bool changeCamera = false;
    bool alreadyActivated = false;
    Vector3f forward;
    Vector3f top;
    Vector3f bottom;

public:
    DashRing();
    DashRing(
        float x,     float y,     float z,
        float dx,    float dy,    float dz,
        float power, float controlLockTime,
        int changeCamera, int rainbow);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
