#ifndef CA_PARTICLE_H
#define CA_PARTICLE_H

class ParticleTexture;

#include "../../toolbox/vector.hpp"
#include "../../particles/particle.hpp"

class CA_Particle : public Particle
{
private:
    void respawn();

public:
    static constexpr float rainRadius = 200.0f;
    static int currentPopulation;
    static Vector3f centerSpawnPos;
    CA_Particle();
    ~CA_Particle();

    bool update();
};
#endif
