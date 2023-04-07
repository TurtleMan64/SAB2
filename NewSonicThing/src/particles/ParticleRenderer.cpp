#include <list>
#include <unordered_map>
#include <glad/glad.h>

#include "../loading/loadergl.hpp"
#include "../toolbox/matrix.hpp"
#include "../models/models.hpp"
#include "particleshader.hpp"
#include "../toolbox/vector.hpp"
#include "particle.hpp"
#include "particletexture.hpp"
#include "particlerenderer.hpp"
#include "particleresources.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../engineTester/main.hpp"
#include "../entities/GreenForest/gfparticle.hpp"


ParticleRenderer::ParticleRenderer(Matrix4f* projectionMatrix)
{
    vboDataBuffer = std::vector<float>(MAX_INSTANCES*INSTANCED_DATA_LENGTH, 0.0f);

    vbo = LoaderGL::createEmptyVBO(INSTANCED_DATA_LENGTH*MAX_INSTANCES);

    std::vector<float> vertices;
    vertices.push_back(-0.5f);
    vertices.push_back( 0.5f);
    vertices.push_back(-0.5f);
    vertices.push_back(-0.5f);
    vertices.push_back( 0.5f);
    vertices.push_back( 0.5f);
    vertices.push_back( 0.5f);
    vertices.push_back(-0.5f);
    quad = new RawModel(LoaderGL::loadToVAO(&vertices, 2)); INCR_NEW("RawModel");

    LoaderGL::addInstancedAttribute(quad->getVaoId(), vbo, 1, 4, INSTANCED_DATA_LENGTH,  0);
    LoaderGL::addInstancedAttribute(quad->getVaoId(), vbo, 2, 4, INSTANCED_DATA_LENGTH,  4);
    LoaderGL::addInstancedAttribute(quad->getVaoId(), vbo, 3, 4, INSTANCED_DATA_LENGTH,  8);
    LoaderGL::addInstancedAttribute(quad->getVaoId(), vbo, 4, 4, INSTANCED_DATA_LENGTH, 12);
    LoaderGL::addInstancedAttribute(quad->getVaoId(), vbo, 5, 4, INSTANCED_DATA_LENGTH, 16);
    LoaderGL::addInstancedAttribute(quad->getVaoId(), vbo, 6, 1, INSTANCED_DATA_LENGTH, 20);

    shader = new ParticleShader; INCR_NEW("ParticleShader");
    shader->start();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
}

void ParticleRenderer::render(
    std::unordered_map<ParticleTexture*, std::list<ParticleStandard*>*>* particlesStandard,
    std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*>* particlesGF,
    Camera* camera, float brightness, int clipSide)
{
    Matrix4f viewMatrix;
    Maths::createViewMatrix(&viewMatrix, camera);
    prepare();
    shader->loadBrightness(brightness);
    
    switch (clipSide + 1)
    {
        case 0: //side -1
        {
            for (auto it = particlesStandard->cbegin(); it != particlesStandard->cend(); it++)
            {
                std::list<ParticleStandard*>* particlesList = it->second;

                vboBufferIdx = 0;
                int numParticlesToRender = 0;
                for (auto particlesIterator = particlesList->cbegin(); particlesIterator != particlesList->cend(); particlesIterator++)
                {
                    ParticleStandard* particle = *particlesIterator;
                    if (particle->positionRef->y < Global::waterHeight)
                    {
                        updateModelViewMatrix(particle, &viewMatrix);
                        updateTexCoordInfo(particle);
                        numParticlesToRender++;
                    }
                }

                if (numParticlesToRender > 0)
                {
                    bindTexture(it->first);

                    float combinedOpacity = it->first->opacity*particlesList->front()->opacity;
                    shader->loadOpacity(combinedOpacity);

                    LoaderGL::updateVBO(vbo, numParticlesToRender*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), numParticlesToRender);
                }
            }
            ANALYSIS_START("GF Particle Render");
            //if (particlesGF.size() >= 1)
            //{
            //    std::list<GF_Particle*>* ls = &particlesGF[ParticleResources::texturePollen];
            //    ls = &particlesGF[ParticleResources::texturePollen];
            //    ls = &particlesGF[ParticleResources::texturePollen];
            //    ls = &particlesGF[ParticleResources::texturePollen];
            //    ls = &particlesGF[ParticleResources::texturePollen];
            //    ls = &particlesGF[ParticleResources::texturePollen];
            //}
            //printf("gf particles = %d\n", (int)particlesGF->size());
            //{
            //    //std::unordered_map<ParticleTexture*, std::list<GF_Particle*>> mymap;
            //
            //    int c = 0;
            //    for (auto const &entry : particlesGF)
            //    {
            //        c++;
            //    }
            //    for (auto const &entry : particlesGF)
            //    {
            //        c++;
            //    }
            //    for (auto const &entry : particlesGF)
            //    {
            //        c++;
            //    }
            //    for (auto const &entry : particlesGF)
            //    {
            //        c++;
            //    }
            //    for (auto const &entry : particlesGF)
            //    {
            //        c++;
            //    }
            //    printf("c %d\n", c);
            //    //int c = 0;
            //    //std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>::iter end = particlesGF->end();
            //
            //    //std::unordered_map<ParticleTexture*, std::list<GF_Particle*>>::iterator it = particlesGF->begin();
            //    //while (it != particlesGF->end())
            //    //{
            //    //    it++;
            //    //    c++;
            //    //}
            //    //
            //    //int v = 0;
            //    //it = particlesGF->begin();
            //    //while (it != particlesGF->end())
            //    //{
            //    //    it++;
            //    //    v++;
            //    //}
            //    //
            //    //int b = 0;
            //    //it = particlesGF->begin();
            //    //while (it != particlesGF->end())
            //    //{
            //    //    it++;
            //    //    b++;
            //    //}
            //    //
            //    //int n = 0;
            //    //it = particlesGF->begin();
            //    //while (it != particlesGF->end())
            //    //{
            //    //    it++;
            //    //    n++;
            //    //}
            //    //
            //    //int m = 0;
            //    //it = particlesGF->begin();
            //    //while (it != particlesGF->end())
            //    //{
            //    //    it++;
            //    //    m++;
            //    //}
            //    //printf("cbv %d%d%d%d%d\n", c, v, b, n, m);
            //}
            //std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*>::iterator it2;
            //for (it2 = particlesGF->begin(); it2 != particlesGF->end(); it2++)
            //{
            //    bindTexture(it2->first);
            //
            //    std::list<GF_Particle*>* particlesList = it2->second;
            //
            //    float combinedOpacity;
            //    bool setOpacity = false;
            //
            //    vboBufferIdx = 0;
            //    int count = 0;
            //    for (std::list<GF_Particle*>::iterator particlesIterator = particlesList->begin(); particlesIterator != particlesList->end(); particlesIterator++)
            //    {
            //        GF_Particle* particle = particlesIterator._Ptr->_Myval;
            //        if (particle->positionRef->y < Global::waterHeight)
            //        {
            //            updateModelViewMatrix(particle->positionRef, particle->rotation, particle->scaleX, particle->scaleY, &viewMatrix);
            //            updateTexCoordInfo(particle);
            //            count++;
            //
            //            if (!setOpacity)
            //            {
            //                combinedOpacity = it2->first->opacity*particle->opacity; //base opacity of the texture * individual particle's opacity
            //                setOpacity = true;
            //            }
            //        }
            //    }
            //    shader->loadOpacity(combinedOpacity);
            //    LoaderGL::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
            //    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            //}
            ANALYSIS_DONE("GF Particle Render");
            break;
        }

        case 1: //side 0
        {
            for (auto it = particlesStandard->cbegin(); it != particlesStandard->cend(); it++)
            {
                std::list<ParticleStandard*>* particlesList = it->second;

                vboBufferIdx = 0;
                int numParticlesToRender = 0;
                for (auto particlesIterator = particlesList->cbegin(); particlesIterator != particlesList->cend(); particlesIterator++)
                {
                    ParticleStandard* particle = *particlesIterator;

                    updateModelViewMatrix(particle, &viewMatrix);
                    updateTexCoordInfo(particle);
                    numParticlesToRender++;
                }

                if (numParticlesToRender > 0)
                {
                    bindTexture(it->first);

                    float combinedOpacity = it->first->opacity*particlesList->front()->opacity;
                    shader->loadOpacity(combinedOpacity);

                    LoaderGL::updateVBO(vbo, numParticlesToRender*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), numParticlesToRender);
                }
            }

            ANALYSIS_START("GF Particle Render");
            for (auto it = particlesGF->cbegin(); it != particlesGF->cend(); it++)
            {
                std::list<GF_Particle*>* particlesList = it->second;

                vboBufferIdx = 0;
                int numParticlesToRender = 0;
                for (auto particlesIterator = particlesList->cbegin(); particlesIterator != particlesList->cend(); particlesIterator++)
                {
                    GF_Particle* particle = *particlesIterator;

                    updateModelViewMatrix(particle, &viewMatrix);
                    updateTexCoordInfo(particle);
                    numParticlesToRender++;
                }

                if (numParticlesToRender > 0)
                {
                    bindTexture(it->first);

                    float combinedOpacity = it->first->opacity*particlesList->front()->opacity;
                    shader->loadOpacity(combinedOpacity);

                    LoaderGL::updateVBO(vbo, numParticlesToRender*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), numParticlesToRender);
                }
            }
            ANALYSIS_DONE("GF Particle Render");
            break;
        }

        case 2: //side 1
        {
            for (auto it = particlesStandard->cbegin(); it != particlesStandard->cend(); it++)
            {
                std::list<ParticleStandard*>* particlesList = it->second;

                vboBufferIdx = 0;
                int numParticlesToRender = 0;
                for (auto particlesIterator = particlesList->cbegin(); particlesIterator != particlesList->cend(); particlesIterator++)
                {
                    ParticleStandard* particle = *particlesIterator;
                    if (particle->positionRef->y >= Global::waterHeight)
                    {
                        updateModelViewMatrix(particle, &viewMatrix);
                        updateTexCoordInfo(particle);
                        numParticlesToRender++;
                    }
                }

                if (numParticlesToRender > 0)
                {
                    bindTexture(it->first);

                    float combinedOpacity = it->first->opacity*particlesList->front()->opacity;
                    shader->loadOpacity(combinedOpacity);

                    LoaderGL::updateVBO(vbo, numParticlesToRender*INSTANCED_DATA_LENGTH, &vboDataBuffer);
                    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), numParticlesToRender);
                }
            }
            ANALYSIS_START("GF Particle Render");
            //std::unordered_map<ParticleTexture*, std::list<GF_Particle*>*>::iterator it2;
            //for (it2 = particlesGF->begin(); it2 != particlesGF->end(); it2++)
            //{
            //    bindTexture(it2->first);
            //
            //    std::list<GF_Particle*>* particlesList = it2->second;
            //
            //    float combinedOpacity;
            //    bool setOpacity = false;
            //
            //    vboBufferIdx = 0;
            //    int count = 0;
            //    for (std::list<GF_Particle*>::iterator particlesIterator = particlesList->begin(); particlesIterator != particlesList->end(); particlesIterator++)
            //    {
            //        GF_Particle* particle = particlesIterator._Ptr->_Myval;
            //        if (particle->positionRef->y >= Global::waterHeight)
            //        {
            //            updateModelViewMatrix(particle->positionRef, particle->rotation, particle->scaleX, particle->scaleY, &viewMatrix);
            //            updateTexCoordInfo(particle);
            //            count++;
            //
            //            if (!setOpacity)
            //            {
            //                combinedOpacity = it2->first->opacity*particle->opacity; //base opacity of the texture * individual particle's opacity
            //                setOpacity = true;
            //            }
            //        }
            //    }
            //    shader->loadOpacity(combinedOpacity);
            //    LoaderGL::updateVBO(vbo, count*INSTANCED_DATA_LENGTH, &vboDataBuffer);
            //    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad->getVertexCount(), count);
            //}
            ANALYSIS_DONE("GF Particle Render");
            break;
        }

        default:
            break;
    }
    
    finishRendering();
}

void ParticleRenderer::cleanUp()
{
    shader->cleanUp();
}

void ParticleRenderer::updateModelViewMatrix(Particle* particle, Matrix4f* viewMatrix)
{
    Matrix4f modelMatrix(false);

    modelMatrix.m00 = viewMatrix->m00;
    modelMatrix.m01 = viewMatrix->m10;
    modelMatrix.m02 = viewMatrix->m20;
    modelMatrix.m03 = 0;
    modelMatrix.m10 = viewMatrix->m01;
    modelMatrix.m11 = viewMatrix->m11;
    modelMatrix.m12 = viewMatrix->m21;
    modelMatrix.m13 = 0;
    modelMatrix.m20 = viewMatrix->m02;
    modelMatrix.m21 = viewMatrix->m12;
    modelMatrix.m22 = viewMatrix->m22;
    modelMatrix.m23 = 0;
    modelMatrix.m30 = particle->positionRef->x;
    modelMatrix.m31 = particle->positionRef->y;
    modelMatrix.m32 = particle->positionRef->z;
    modelMatrix.m33 = 1;

    modelMatrix.m00 *= particle->scale;
    modelMatrix.m01 *= particle->scale;
    modelMatrix.m02 *= particle->scale;
    modelMatrix.m03 *= particle->scale;
    modelMatrix.m10 *= particle->scale;
    modelMatrix.m11 *= particle->scale;
    modelMatrix.m12 *= particle->scale;
    modelMatrix.m13 *= particle->scale;

    viewMatrix->multiply(&modelMatrix, &modelMatrix);
    storeMatrixData(&modelMatrix);
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
    vboDataBuffer[vboBufferIdx] = particle->texOffset1.x; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->texOffset1.y; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->texOffset2.x; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->texOffset2.y; vboBufferIdx++;
    vboDataBuffer[vboBufferIdx] = particle->blend;        vboBufferIdx++;
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
    glBindTexture(GL_TEXTURE_2D, texture->textureId);
    shader->loadGlow(texture->glow);
    shader->loadNumberOfRows((float)texture->numberOfRows);
}

void ParticleRenderer::prepare()
{
    shader->start();
    glBindVertexArray(quad->getVaoId());
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
    glEnable(GL_CULL_FACE);
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
