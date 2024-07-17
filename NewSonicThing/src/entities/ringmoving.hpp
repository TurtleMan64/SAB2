#ifndef RINGMOVING_H
#define RINGMOVING_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include "../toolbox/vector.hpp"

//Ring that is not a chunked entity. This means it can move around anywhere.
class RingMoving : public Entity
{
private:
    Vector3f vel;
    bool moves;
    float ageTimer;
    bool trackingPlayer;
    float trackingPlayerTimer;

public:
    RingMoving();
    RingMoving(float x, float y, float z); //create if normal ring gets close enough to trigger magnet shield
    RingMoving(float x, float y, float z, float xVel, float yVel, float zVel);

    void step();

    std::vector<TexturedModel*>* getModels();

    inline bool canLightdashOn() const;
};
#endif
