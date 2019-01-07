#include "waterrenderer.h"
#include "../renderEngine/renderEngine.h"
#include "watershader.h"
#include "waterframebuffers.h"
#include "watertile.h"
#include <list>
#include <vector>
#include "../engineTester/main.h"
#include "../toolbox/maths.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../shadows/shadowmapmasterrenderer.h"

const float WaterRenderer::WAVE_SPEED = 0.0002f;

WaterRenderer::WaterRenderer(WaterShader* shader, Matrix4f* projectionMatrix, WaterFrameBuffers* fbos, ShadowMapMasterRenderer* shadowMapRenderer)
{
	this->shader = shader;
	this->fbos = fbos;
	this->shadowMapRenderer = shadowMapRenderer;
	dudvTexture = Loader::loadTexture("res/Images/waterDUDV.png");
	normalMap   = Loader::loadTexture("res/Images/normalMap.png");
	shader->start();
	shader->connectTextureUnits();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
	setUpVAO();
}

void WaterRenderer::prepareRender(Camera* camera, Light* sun)
{
	shader->start();
	shader->loadViewMatrix(camera);
	if (Global::renderShadowsFar)
	{
		shader->loadToShadowSpaceMatrix(shadowMapRenderer->getToShadowMapSpaceMatrix());
	}
	moveFactor += WAVE_SPEED;
	moveFactor = fmodf(moveFactor, 1);
	shader->loadMoveFactor(moveFactor);
	shader->loadLight(sun);
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture  (GL_TEXTURE_2D, fbos->getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture  (GL_TEXTURE_2D, fbos->getRefractionTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture  (GL_TEXTURE_2D, dudvTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture  (GL_TEXTURE_2D, normalMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture  (GL_TEXTURE_2D, fbos->getRefractionDepthTexture());
	glDepthMask(true);
}

void WaterRenderer::unbind()
{
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->stop();
}

void WaterRenderer::setUpVAO()
{
	// Just x and z vectex positions here, y is set to 0 in v.shader
	std::vector<float> vertices;
	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);

	quad = new RawModel(Loader::loadToVAO(&vertices, 2));
	INCR_NEW
}

void WaterRenderer::render(std::list<WaterTile*>* water, Camera* camera, Light* sun)
{
	prepareRender(camera, sun);
	for (WaterTile* tile : (*water))
	{
		Matrix4f modelMatrix;
		Vector3f tilePosition(tile->getX(), tile->getHeight(), tile->getZ());
		Maths::createTransformationMatrix(&modelMatrix, &tilePosition, 0, 0, 0, 0, WaterTile::TILE_SIZE);
		shader->loadModelMatrix(&modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, quad->getVertexCount());
	}
	unbind();
}

void WaterRenderer::updateProjectionMatrix(Matrix4f* projectionMatrix)
{
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
}
