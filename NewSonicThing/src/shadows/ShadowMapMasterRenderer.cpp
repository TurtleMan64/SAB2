#include "shadowmapmasterrenderer.hpp"
#include "shadowshader.hpp"
#include "shadowbox.hpp"
#include "shadowframebuffer.hpp"
#include "shadowmapentityrenderer.hpp"
#include "../toolbox/matrix.hpp"
#include "../toolbox/vector.hpp"
#include "../entities/camera.hpp"
#include "../entities/light.hpp"
#include "../entities/entity.hpp"
#include "../engineTester/main.hpp"

ShadowMapMasterRenderer::ShadowMapMasterRenderer()
{
    projectionMatrix       = new Matrix4f; INCR_NEW("Matrix4f");
    lightViewMatrix        = new Matrix4f; INCR_NEW("Matrix4f");
    projectionViewMatrix   = new Matrix4f; INCR_NEW("Matrix4f");
    toShadowMapSpaceMatrix = new Matrix4f; INCR_NEW("Matrix4f");
    createOffset();

    shader = new ShadowShader("res/Shaders/shadows/shadowVertexShader.txt", "res/Shaders/shadows/shadowFragmentShader.txt"); INCR_NEW("ShadowShader");
    shadowBox = new ShadowBox(lightViewMatrix, Global::gameCamera); INCR_NEW("ShadowBox");
    shadowFbo = new ShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE); INCR_NEW("ShadowFrameBuffer");
    entityRenderer = new ShadowMapEntityRenderer(shader, projectionViewMatrix); INCR_NEW("ShadowMapEntityRenderer");
}

void ShadowMapMasterRenderer::render(std::unordered_map<TexturedModel*, std::list<Entity*>>* entities, Light* sun)
{
    shadowBox->update();
    //Vector3f* sunPosition = sun->getPosition();
    //Vector3f lightDirection(-sunPosition->x, -sunPosition->y, -sunPosition->z);
    prepare(&sun->direction, shadowBox);
    entityRenderer->render(entities);
    finish();
}

Matrix4f* ShadowMapMasterRenderer::getToShadowMapSpaceMatrix()
{
    offset->multiply(projectionViewMatrix, toShadowMapSpaceMatrix);

    return toShadowMapSpaceMatrix;
}

void ShadowMapMasterRenderer::cleanUp()
{
    shader->cleanUp();
    shadowFbo->cleanUp();
}

GLuint ShadowMapMasterRenderer::getShadowMap()
{
    return shadowFbo->getShadowMap();
}

Matrix4f* ShadowMapMasterRenderer::getLightSpaceTransform()
{
    return lightViewMatrix;
}

void ShadowMapMasterRenderer::prepare(Vector3f* lightDirection, ShadowBox* box)
{
    updateOrthoProjectionMatrix(box->getWidth(), box->getHeight(), box->getLength());
    Vector3f cent = box->getCenter();
    updateLightViewMatrix(lightDirection, &cent);
    projectionMatrix->multiply(lightViewMatrix, projectionViewMatrix);
    shadowFbo->bindFrameBuffer();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    shader->start();
}

void ShadowMapMasterRenderer::finish()
{
    shader->stop();
    shadowFbo->unbindFrameBuffer();
}

void ShadowMapMasterRenderer::updateLightViewMatrix(Vector3f* direction, Vector3f* center)
{
    Vector3f dir(direction);
    Vector3f cent(center);

    dir.normalize();
    cent.neg();
    lightViewMatrix->setIdentity();

    Vector2f r(dir.x, dir.z);

    float pitch = acosf(r.length());

    Vector3f axis(1, 0, 0);
    lightViewMatrix->rotate(pitch, &axis);

    float yaw = Maths::toDegrees((atanf(dir.x / dir.z)));
    yaw = dir.z > 0 ? yaw - 180 : yaw;

    axis.set(0, 1, 0);
    lightViewMatrix->rotate(-Maths::toRadians(yaw), &axis);

    lightViewMatrix->translate(&cent);
}

void ShadowMapMasterRenderer::updateOrthoProjectionMatrix(float width, float height, float length)
{
    projectionMatrix->setIdentity();
    projectionMatrix->m00 = 2.0f / width;
    projectionMatrix->m11 = 2.0f / height;
    projectionMatrix->m22 = -2.0f / length;
    projectionMatrix->m33 = 1;
}

void ShadowMapMasterRenderer::createOffset()
{
    offset = new Matrix4f; INCR_NEW("Matrix4f");
    Vector3f trans(0.5f, 0.5f, 0.5f);
    offset->translate(&trans);
    offset->scale(&trans);
}
