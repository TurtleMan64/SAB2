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


ParticleRenderer::ParticleRenderer(Matrix4f* projectionMatrix)
{
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
	shader = new ParticleShader; INCR_NEW("ParticleShader");
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
}

void ParticleRenderer::render(std::unordered_map<ParticleTexture*, std::list<Particle*>>* particles, Camera* camera, float brightness, int clipSide)
{
	Matrix4f viewMatrix;
	Maths::createViewMatrix(&viewMatrix, camera);
	prepare();
	shader->loadBrightness(brightness);

	switch (clipSide + 1)
	{
		case 0: //side -1
			for (auto texture : (*particles))
			{
				//bind texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.first->getTextureID());
				//shader->loadOpacity(texture.first->getOpacity());
                float texOpac = texture.first->getOpacity();
				shader->loadGlow(texture.first->getGlow());
				std::list<Particle*>* texturesList = &texture.second;
				for (Particle* particle : (*texturesList))
				{
					if (particle->getPosition()->y < 0)
					{
                        shader->loadOpacity(texOpac*particle->getOpacity());
						updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
						shader->loadTextureCoordInfo(particle->getTexOffset1(), particle->getTexOffset2(), (float)texture.first->getNumberOfRows(), particle->getBlend());
						glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
					}
				}
			}
			break;

		case 1: //side 0
			for (auto texture : (*particles))
			{
				//bind texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.first->getTextureID());
				//shader->loadOpacity(texture.first->getOpacity());
				shader->loadGlow(texture.first->getGlow());
                float texOpac = texture.first->getOpacity();

				std::list<Particle*>* texturesList = &texture.second;
				for (Particle* particle : (*texturesList))
				{
                    shader->loadOpacity(texOpac*particle->getOpacity());
					updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
					shader->loadTextureCoordInfo(particle->getTexOffset1(), particle->getTexOffset2(), (float)texture.first->getNumberOfRows(), particle->getBlend());
					glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
				}
			}
			break;

		case 2: //side 1
			for (auto texture : (*particles))
			{
				//bind texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.first->getTextureID());
				//shader->loadOpacity(texture.first->getOpacity());
				shader->loadGlow(texture.first->getGlow());
                float texOpac = texture.first->getOpacity();
				std::list<Particle*>* texturesList = &texture.second;
				for (Particle* particle : (*texturesList))
				{
					if (particle->getPosition()->y >= 0)
					{
                        shader->loadOpacity(texOpac*particle->getOpacity());
						updateModelViewMatrix(particle->getPosition(), particle->getRotation(), particle->getScaleX(), particle->getScaleY(), &viewMatrix);
						shader->loadTextureCoordInfo(particle->getTexOffset1(), particle->getTexOffset2(), (float)texture.first->getNumberOfRows(), particle->getBlend());
						glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
					}
				}
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
	shader->loadModelViewMatrix(&modelViewMatrix);
}

void ParticleRenderer::updateProjectionMatrix(Matrix4f* projectionMatrix)
{
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
}

void ParticleRenderer::prepare()
{
	shader->start();
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
}

void ParticleRenderer::finishRendering()
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->stop();
}
