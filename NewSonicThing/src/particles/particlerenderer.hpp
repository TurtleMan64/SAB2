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
class IC_Particle;
class CA_Particle;

#include <list>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>

class ParticleRenderer
{
private:
    RawModel* quad = nullptr;
    ParticleShader* shader = nullptr;

    static constexpr int MAX_INSTANCES = 4200;
    static constexpr int INSTANCED_DATA_LENGTH = 21;

    std::vector<float> vboDataBuffer;
    int vboBufferIdx = 0;

    GLuint vbo = GL_NONE;

    void prepare();

    void bindTexture(ParticleTexture* texture);

    void finishRendering();

    void updateModelViewMatrix(Particle* particle, Matrix4f* viewMatrix);

    void storeMatrixData(Matrix4f* matrix);

    void updateTexCoordInfo(Particle* particle);

public:
    ParticleRenderer(Matrix4f* projectionMatrix);

    void render(std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>*>* particlesStandard, 
                std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*>* particlesGF,
                std::unordered_map<ParticleTexture*, std::list<IC_Particle*>*>* particlesIC,
                std::unordered_map<ParticleTexture*, std::list<CA_Particle*>*>* particlesCA,
                Camera* camera, float brightness, int clipSide);

    void updateProjectionMatrix(Matrix4f* projectionMatrix);

    void cleanUp();
};
#endif
