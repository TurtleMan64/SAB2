#include <list>
#include <unordered_map>
#include <glad/glad.h>

#include "../renderEngine/renderEngine.h"
#include "../toolbox/matrix.h"
#include "../models/models.h"
#include "particleshader.h"
#include "../toolbox/vector.h"
#include "particle.h"
#include "particletexture.h"
#include "particlerenderer.h"
#include "../toolbox/maths.h"
#include "../entities/camera.h"
#include "../engineTester/main.h"
#include "../entities/GreenForest/gfparticle.h"


ParticleRenderer::ParticleRenderer(Matrix4f* projectionMatrix)
{
    vboDataBuffer = std::vector<float>(MAX_INSTANCES*INSTANCED_DATA_LENGTH, 0.0f);

    vbo = Loader::createEmptyVBO(INSTANCED_DATA_LENGTH*MAX_INSTANCES);

    std::vector<float> vertices;
    vertices.push_back(-0.5f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.5f);
    vertices.push_back(0.5f);
    vertices.push_back(0.5f);
    vertices.push_back(0.5f);
    vertices.push_back(-0.5f);
    quad = new RawModel(Loader::loadToVAO(&vertices, 2)); INCR_NEW("RawModel");

    Loader::addInstancedAttribute(quad->getVaoID(), vbo, 1, 4, INSTANCED_DATA_LENGTH,  0);
    Loader::addInstancedAttribute(quad->getVaoID(), vbo, 2, 4, INSTANCED_DATA_LENGTH,  4);
    Loader::addInstancedAttribute(quad->getVaoID(), vbo, 3, 4, INSTANCED_DATA_LENGTH,  8);
    Loader::addInstancedAttribute(quad->getVaoID(), vbo, 4, 4, INSTANCED_DATA_LENGTH, 12);
    Loader::addInstancedAttribute(quad->getVaoID(), vbo, 5, 4, INSTANCED_DATA_LENGTH, 16);
    Loader::addInstancedAttribute(quad->getVaoID(), vbo, 6, 1, INSTANCED_DATA_LENGTH, 20);

    shader = new ParticleShader; INCR_NEW("ParticleShader");
    shader->start();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
}

void ParticleRenderer::render(
    std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>>* particlesStandard,
    std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>* particlesGF,
    Camera* camera, float brightness, int clipSide)
{
    Matrix4f viewMatrix;
    Maths::createViewMatrix(&viewMatrix, camera);
    prepare();
    shader->loadBrightness(brightness);
    
    switch (clipSide + 1)
    {
        case 0: //side -1
            for (auto texture : (*particlesStandard))
            {
                bindTexture(texture.first);
                float texOpac = texture.first->getOpacity();

                std::list<ParticleStandard*>* texturesList = &texture.second;

                vboBufferIdx = 0;
                int count = 0;
                for (ParticleStandard* particle : (*texturesList))
                {
                    if (particle->getPosition()->y < Global::waterHeight)
                    {
                        shader->loadOpacity(texOpac*particle->getOpacity());
                        updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
                        updateTexCoordInfo(particle);
                        count++;
                    }
                }
                Loader::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            }
            for (auto texture : (*particlesGF))
            {
                bindTexture(texture.first);
                float texOpac = texture.first->getOpacity();

                std::list<GF_Particle*>* texturesList = &texture.second;

                vboBufferIdx = 0;
                int count = 0;
                for (GF_Particle* particle : (*texturesList))
                {
                    if (particle->getPosition()->y < Global::waterHeight)
                    {
                        shader->loadOpacity(texOpac*particle->getOpacity());
                        updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
                        updateTexCoordInfo(particle);
                        count++;
                    }
                }
                Loader::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            }
            break;

        case 1: //side 0
            for (auto texture : (*particlesStandard))
            {
                bindTexture(texture.first);
                float texOpac = texture.first->getOpacity();

                std::list<ParticleStandard*>* texturesList = &texture.second;

                vboBufferIdx = 0;
                int count = 0;
                for (ParticleStandard* particle : (*texturesList))
                {
                    shader->loadOpacity(texOpac*particle->getOpacity());
                    updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
                    updateTexCoordInfo(particle);
                    count++;
                }
                Loader::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            }
            for (auto texture : (*particlesGF))
            {
                bindTexture(texture.first);
                float texOpac = texture.first->getOpacity();

                std::list<GF_Particle*>* texturesList = &texture.second;

                vboBufferIdx = 0;
                int count = 0;
                for (GF_Particle* particle : (*texturesList))
                {
                    shader->loadOpacity(texOpac*particle->getOpacity());
                    updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
                    updateTexCoordInfo(particle);
                    count++;
                }
                Loader::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            }
            break;

        case 2: //side 1
            for (auto texture : (*particlesStandard))
            {
                bindTexture(texture.first);
                float texOpac = texture.first->getOpacity();

                std::list<ParticleStandard*>* texturesList = &texture.second;

                vboBufferIdx = 0;
                int count = 0;
                for (ParticleStandard* particle : (*texturesList))
                {
                    if (particle->getPosition()->y >= Global::waterHeight)
                    {
                        shader->loadOpacity(texOpac*particle->getOpacity());
                        updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
                        updateTexCoordInfo(particle);
                        count++;
                    }
                }
                Loader::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            }
            for (auto texture : (*particlesGF))
            {
                bindTexture(texture.first);
                float texOpac = texture.first->getOpacity();

                std::list<GF_Particle*>* texturesList = &texture.second;

                vboBufferIdx = 0;
                int count = 0;
                for (GF_Particle* particle : (*texturesList))
                {
                    if (particle->getPosition()->y >= Global::waterHeight)
                    {
                        shader->loadOpacity(texOpac*particle->getOpacity());
                        updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
                        updateTexCoordInfo(particle);
                        count++;
                    }
                }
                Loader::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            }
            break;

        default:
            break;
    }
    
    finishRendering();
}

void ParticleRenderer::cleanUp()
{
    shader->cleanUp();
}

void ParticleRenderer::updateModelViewMatrix(Vector3f* position, float rotation, float scaleX, float scaleY, Matrix4f* viewMatrix)
{
    Matrix4f modelMatrix;
    modelMatrix.translate(position);
    modelMatrix.m00 = viewMatrix->m00;
    modelMatrix.m01 = viewMatrix->m10;
    modelMatrix.m02 = viewMatrix->m20;
    modelMatrix.m10 = viewMatrix->m01;
    modelMatrix.m11 = viewMatrix->m11;
    modelMatrix.m12 = viewMatrix->m21;
    modelMatrix.m20 = viewMatrix->m02;
    modelMatrix.m21 = viewMatrix->m12;
    modelMatrix.m22 = viewMatrix->m22;
    Vector3f axis(0, 0, 1);
    modelMatrix.rotate(Maths::toRadians(rotation), &axis);
    Vector3f scaleVec(scaleX, scaleY, 1);
    modelMatrix.scale(&scaleVec);
    Matrix4f modelViewMatrix = Matrix4f(modelMatrix);
    viewMatrix->multiply(&modelViewMatrix, &modelViewMatrix);
    storeMatrixData(&modelViewMatrix);
    //shader->loadModelViewMatrix(&modelViewMatrix);
}

void ParticleRenderer::storeMatrixData(Matrix4f* matrix)
{
    vboDataBuffer[vboBufferIdx] = matrix->m00; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m01; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m02; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m03; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m10; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m11; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m12; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m13; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m20; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m21; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m22; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m23; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m30; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m31; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m32; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = matrix->m33; vboBufferIdx++;
}

void ParticleRenderer::updateTexCoordInfo(Particle* particle)
{
    vboDataBuffer[vboBufferIdx] = particle->getTexOffset1()->x; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->getTexOffset1()->y; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->getTexOffset2()->x; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->getTexOffset2()->y; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->getBlend();         vboBufferIdx++;
}

void ParticleRenderer::updateProjectionMatrix(Matrix4f* projectionMatrix)
{
    shader->start();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
}

void ParticleRenderer::bindTexture(ParticleTexture* texture)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    shader->loadGlow(texture->getGlow());
    shader->loadNumberOfRows((float)texture->getNumberOfRows());
}

void ParticleRenderer::prepare()
{
    shader->start();
    glBindVertexArray(quad->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);
    glDisable(GL_CULL_FACE);
}

void ParticleRenderer::finishRendering()
{
    glDepthMask(true);
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glBindVertexArray(0);
    shader->stop();
}
