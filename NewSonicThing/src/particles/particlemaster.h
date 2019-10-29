#ifndef PARTICLEMASTER_H
#define PARTICLEMASTER_H

class Matrix4f;
class Camera;
class Vector3f;
class ParticleTexture;
class Particle;
class ParticleRenderer;

#include <unordered_map>
#include <list>

class ParticleMaster
{
private:
    static std::unordered_map<ParticleTexture*, std::list<Particle*>> particles;
    static ParticleRenderer* renderer;

public:
    static void init(Matrix4f* projectionMatrix);

    static void update(Camera* cam);

    static void renderParticles(Camera* camera, float brightness, int clipSide);

    static void cleanUp();

    static void createParticle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, bool onlyRendersOnce);

    static void createParticle(ParticleTexture* texture, Vector3f* position, float lifeLength, float scale, float opacity, bool onlyRendersOnce);

    static void createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float lifeLength, float scale, bool onlyRendersOnce);

    static void createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float rotation, float scale, float scaleChange, bool posIsRef, bool onlyRendersOnce, float opacity);

    static void createParticle(ParticleTexture* texture, Vector3f* position, Vector3f* velocity, float gravityEffect,
        float lifeLength, float rotation, float scaleX, float scaleXChange, float scaleY, float scaleYChange, 
        bool posIsRef, bool onlyRendersOnce);

    static void addParticle(Particle* particle);

    static void updateProjectionMatrix(Matrix4f* projectionMatrix);

    static void deleteAllParticles();
};

#endif
