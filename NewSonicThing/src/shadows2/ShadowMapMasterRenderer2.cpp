#include "shadowmapmasterrenderer2.h"
#include "shadowshader2.h"
#include "shadowbox2.h"
#include "shadowframebuffer2.h"
#include "shadowmapentityrenderer2.h"
#include "../toolbox/matrix.h"
#include "../toolbox/vector.h"
#include "../entities/camera.h"
#include "../entities/light.h"
#include "../entities/entity.h"
#include "../engineTester/main.h"

ShadowMapMasterRenderer2::ShadowMapMasterRenderer2()
{
    projectionMatrix       = new Matrix4f; INCR_NEW("Matrix4f")
    lightViewMatrix        = new Matrix4f; INCR_NEW("Matrix4f")
    projectionViewMatrix   = new Matrix4f; INCR_NEW("Matrix4f")
    toShadowMapSpaceMatrix = new Matrix4f; INCR_NEW("Matrix4f")
    createOffset();

    shader = new ShadowShader2("res/Shaders/shadows2/shadowVertexShader2.txt", "res/Shaders/shadows2/shadowFragmentShader2.txt"); INCR_NEW("ShadowShader2")
    shadowBox = new ShadowBox2(lightViewMatrix, Global::gameCamera); INCR_NEW("ShadowBox2")
    shadowFbo = new ShadowFrameBuffer2(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE); INCR_NEW("ShadowFrameBuffer2")
    entityRenderer = new ShadowMapEntityRenderer2(shader, projectionViewMatrix); INCR_NEW("ShadowMapEntityRenderer2")
}

void ShadowMapMasterRenderer2::render(std::unordered_map<TexturedModel*, std::list<Entity*>>* entities, Light* sun)
{
    shadowBox->update();
    //Vector3f* sunPosition = sun->getPosition();
    //Vector3f lightDirection(-sunPosition->x, -sunPosition->y, -sunPosition->z);
    prepare(&sun->direction, shadowBox);
    entityRenderer->render(entities);
    finish();
}

Matrix4f* ShadowMapMasterRenderer2::getToShadowMapSpaceMatrix()
{
    offset->multiply(projectionViewMatrix, toShadowMapSpaceMatrix);

    return toShadowMapSpaceMatrix;
}

void ShadowMapMasterRenderer2::cleanUp()
{
    shader->cleanUp();
    shadowFbo->cleanUp();
}

GLuint ShadowMapMasterRenderer2::getShadowMap()
{
    return shadowFbo->getShadowMap();
}

Matrix4f* ShadowMapMasterRenderer2::getLightSpaceTransform()
{
    return lightViewMatrix;
}

void ShadowMapMasterRenderer2::prepare(Vector3f* lightDirection, ShadowBox2* box)
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

void ShadowMapMasterRenderer2::finish()
{
    shader->stop();
    shadowFbo->unbindFrameBuffer();
}

void ShadowMapMasterRenderer2::updateLightViewMatrix(Vector3f* direction, Vector3f* center)
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

void ShadowMapMasterRenderer2::updateOrthoProjectionMatrix(float width, float height, float length)
{
    projectionMatrix->setIdentity();
    projectionMatrix->m00 = 2.0f / width;
    projectionMatrix->m11 = 2.0f / height;
    projectionMatrix->m22 = -2.0f / length;
    projectionMatrix->m33 = 1;
}

void ShadowMapMasterRenderer2::createOffset()
{
    offset = new Matrix4f; INCR_NEW("Matrix4f")
    Vector3f trans(0.5f, 0.5f, 0.5f);
    offset->translate(&trans);
    offset->scale(&trans);
}
