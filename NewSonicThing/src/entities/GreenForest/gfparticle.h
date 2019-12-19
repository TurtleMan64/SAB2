#ifndef GF_PARTICLE_H
#define GF_PARTICLE_H

class ParticleTexture;

#include "../../toolbox/vector.h"
#include "../../particles/particle.h"

class GF_Particle : public Particle
{
private:
    void respawn();

public:
    static const float pollenRadius;
    static int currentPopulation;
    static Vector3f centerSpawnPos;
    GF_Particle();
    ~GF_Particle();

    bool update();
};
#endif
