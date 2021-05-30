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
#include <vector>
#include <unordered_map>
#include <glad/glad.h>

class ParticleRenderer
{
private:
    RawModel* quad;
    ParticleShader* shader;

    const int MAX_INSTANCES = 10000;
    const int INSTANCED_DATA_LENGTH = 21;

    std::vector<float> vboDataBuffer;
    int vboBufferIdx = 0;

    GLuint vbo;

    void prepare();

    void bindTexture(ParticleTexture* texture);

    void finishRendering();

    void updateModelViewMatrix(Vector3f* position, float rotation, float scaleX, float scaleY, Matrix4f* viewMatrix);

    void storeMatrixData(Matrix4f* matrix);

    void updateTexCoordInfo(Particle* particle);

public:
    ParticleRenderer(Matrix4f* projectionMatrix);

    void render(std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>>* particlesStandard, 
                std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>* particlesGF,
                Camera* camera, float brightness, int clipSide);

    void updateProjectionMatrix(Matrix4f* projectionMatrix);

    void cleanUp();
};
#endif
