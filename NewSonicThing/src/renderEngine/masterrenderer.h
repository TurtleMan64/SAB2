#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

class Entity;
class Matrix4f;
class Light;
class Camera;
class ShadowMapMasterRenderer;
class ShadowMapMasterRenderer2;
class EntityShader;
class EntityRenderer;
class TexturedModel;

#include <glad/glad.h>
#include <unordered_map>
#include <list>

class MasterRenderer
{
private:
    static EntityShader* entityShader;
    static EntityRenderer* entityRenderer;
    static ShadowMapMasterRenderer* shadowMapRenderer;
    static ShadowMapMasterRenderer2* shadowMapRenderer2;

    static std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMap;
    static std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapPass2;
    static std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapPass3;
    static std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapNoDepth;
    static std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapTransparent;
    static std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapPass6;

    static GLuint randomMap;
    static GLuint transparentFrameBuffer;
    static GLuint transparentDepthTexture;

    static void prepareNormalRender();

    static void prepareTransparentRender();

    static void prepareRenderDepthOnly();

public:
    static const float NEAR_PLANE;
    static const float FAR_PLANE;
    static float VFOV_BASE; // Vertical Field of View
    static float VFOV_ADDITION; // Additional Field of View from kart speed

    static Matrix4f* projectionMatrix;

    static GLuint currentBoundFramebuffer;

    static void init();

    static void render(Camera* camera, float clipX, float clipY, float clipZ, float clipW, float waterBlendAmount);

    static void cleanUp();

    static void processEntity(Entity* entity);

    static void clearAllEntities();

    static void enableCulling();

    static void disableCulling();

    static void makeProjectionMatrix();

    static float getVFOV();

    static GLuint getShadowMapTexture();

    static ShadowMapMasterRenderer* getShadowRenderer();

    static GLuint getShadowMapTexture2();

    static ShadowMapMasterRenderer2* getShadowRenderer2();

    static void renderShadowMaps(Light* sun);
};
#endif
