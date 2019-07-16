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
#include "../entities/camera.h"

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

	//calc behind clipm plane based on camera
	Vector3f camDir = camera->target - camera->eye;
	camDir.normalize();
	camDir.neg();
	Vector3f startPos(&camera->eye);
	//startPos = startPos + camDir.scaleCopy(-100);
	Vector4f plane = Maths::calcPlaneValues(&startPos, &camDir);
	shader->loadClipPlaneBehind(&plane);


	if (Global::renderShadowsFar)
	{
		shader->loadToShadowSpaceMatrix(shadowMapRenderer->getToShadowMapSpaceMatrix());
	}
	moveFactor += WAVE_SPEED;
	moveFactor = fmodf(moveFactor, 1);
	shader->loadMoveFactor(moveFactor);
	shader->loadLight(sun);
	shader->loadWaterHeight(Global::waterHeight);
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
	glEnable(GL_CLIP_DISTANCE1);
}

void WaterRenderer::unbind()
{
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glDisable(GL_CLIP_DISTANCE1);
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

	quad = new RawModel(Loader::loadToVAO(&vertices, 2)); INCR_NEW("RawModel");
}

void WaterRenderer::render(std::vector<WaterTile*>* water, Camera* camera, Light* sun)
{
	prepareRender(camera, sun);
    float xOff = WaterTile::TILE_SIZE*((int)(camera->eye.x/WaterTile::TILE_SIZE));
    float zOff = WaterTile::TILE_SIZE*((int)(camera->eye.z/WaterTile::TILE_SIZE));
	for (WaterTile* tile : (*water))
	{
		Matrix4f modelMatrix;
		Vector3f tilePosition(tile->getX() + xOff, Global::waterHeight, tile->getZ() + zOff);
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
