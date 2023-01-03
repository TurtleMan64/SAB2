#ifndef DASHPAD_H
#define DASHPAD_H

class TexturedModel;

#include <list>
#include "entity.hpp"


class Dashpad : public Entity
{
private:
    static std::vector<TexturedModel*> models;
    bool playerIsIn = false;
    float power = 100.0f;
    float controlLockTime = 1.0f;
    float camScale = 1.0f;
    Vector3f forward;
    Vector3f up;

public:
    Dashpad();
    Dashpad(
        float x,     float y,     float z,
        float power, float controlLockTime,
        float rotX, float rotY, float rotZ,
        float camScale);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
