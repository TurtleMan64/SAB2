#ifndef PARTICLEMASTER_H
#define PARTICLEMASTER_H

class Matrix4f;
class Camera;
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

	static void addParticle(Particle* particle);

	static void updateProjectionMatrix(Matrix4f* projectionMatrix);

	static void deleteAllParticles();
};

#endif
