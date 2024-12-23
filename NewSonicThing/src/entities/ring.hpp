#ifndef RING_H
#define RING_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class Ring : public Entity
{
public:
    static std::vector<TexturedModel*> models;

private:
    float xVel;
    float yVel;
    float zVel;
    bool moves;
    int grabTimer;
    bool givesPoints;

public:
    Ring();
    Ring(float x, float y, float z);
    Ring(float x, float y, float z, float xVel, float yVel, float zVel);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();

    inline bool canLightdashOn() const;
};
#endif
