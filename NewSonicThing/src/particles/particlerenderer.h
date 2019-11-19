#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

class Matrix4f;
class RawModel;
class ParticleShader;
class Vector3f;
class Particle;
class ParticleTexture;
class Camera;
class GF_Particle;


#include "../renderEngine/renderEngine.h"
#include <list>
#include <unordered_map>
#include <glad/glad.h>

class ParticleRenderer
{
private:
    RawModel* quad;
    ParticleShader* shader;

    void prepare();

    void finishRendering();

    void updateModelViewMatrix(Vector3f* position, float rotation, float scaleX, float scaleY, Matrix4f* viewMatrix);

public:
    ParticleRenderer(Matrix4f* projectionMatrix);

    void render(std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>>* particlesStandard, 
                std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>* particlesGF,
                Camera* camera, float brightness, int clipSide);

    void updateProjectionMatrix(Matrix4f* projectionMatrix);

    void cleanUp();
};
#endif
