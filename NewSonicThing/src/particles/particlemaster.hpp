#ifndef PARTICLEMASTER_H
#define PARTICLEMASTER_H

class Matrix4f;
class Camera;
class Vector3f;
class ParticleTexture;
class ParticleStandard;
class GF_Particle;
class ParticleRenderer;

#include <unordered_map>
#include <list>

class ParticleMaster
{
private:
    static std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>*> particlesStandard;
    static std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*> particlesGF;
    static ParticleRenderer* renderer;

public:
    static void init(Matrix4f* projectionMatrix);

    static void update(Camera* cam);

    static void renderParticles(Camera* camera, float brightness, int clipSide);

    static void cleanUp();

    static void createParticle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce, bool optional);

    static void createParticle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce, bool optional);

    static void createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce, bool optional);

    static void createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity, bool optional);

    static void createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, bool optional);

    static void addParticleStandard(ParticleStandard* particle);

    static void addParticleGF(GF_Particle* particle);

    static void updateProjectionMatrix(Matrix4f* projectionMatrix);

    static void deleteAllParticles();
};

#endif
