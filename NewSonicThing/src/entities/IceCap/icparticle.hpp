#ifndef IC_PARTICLE_H
#define IC_PARTICLE_H

class ParticleTexture;

#include "../../toolbox/vector.hpp"
#include "../../particles/particle.hpp"

class IC_Particle : public Particle
{
private:
    void respawn();

public:
    static constexpr float snowRadius = 200.0f;
    static int currentPopulation;
    static Vector3f centerSpawnPos;
    IC_Particle();
    ~IC_Particle();

    bool update();
};
#endif
