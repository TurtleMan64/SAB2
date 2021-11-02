#ifndef HINTMONITOR_H
#define HINTMONITOR_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

class HintMonitor : public Entity
{
private:
    static std::list<TexturedModel*> models;

    Vector3f hintPiecePos;
    Vector3f hintEyePos;
    Vector3f storedPlayerPos;
    float readingTimer = 0.0f;
    float fadeoutTimer = 0.0f;

    bool collected = false;

public:
    HintMonitor();
    HintMonitor(float x, float y, float z, float rotationY);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
