#ifndef GF_PARTICLE_H
#define GF_PARTICLE_H

class ParticleTexture;

#include "../../toolbox/vector.hpp"
#include "../../particles/particle.hpp"

class GF_Particle : public Particle
{
private:
    void respawn();

public:
    static constexpr float pollenRadius = 200.0f;
    static int currentPopulation;
    static Vector3f centerSpawnPos;
    GF_Particle();
    ~GF_Particle();

    bool update();
};
#endif
