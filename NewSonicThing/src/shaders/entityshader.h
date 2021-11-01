#ifndef ENTITYSHADER_H
#define ENTITYSHADER_H

class Matrix4f;
class Camera;
class Light;
class Vector3f;

#include <glad/glad.h>

class EntityShader
{
private:
    GLuint programId        = GL_NONE;
    GLuint vertexShaderId   = GL_NONE;
    GLuint fragmentShaderId = GL_NONE;

    int location_transformationMatrix   = 0;
    int location_projectionMatrix       = 0;
    int location_viewMatrix             = 0;
    int location_sunDirInv              = 0;
    int location_sunColor               = 0;
    int location_shineDamper            = 0;
    int location_reflectivity           = 0;
    int location_useFakeLighting        = 0;
    int location_hasTransparency        = 0;
    int location_baseColour             = 0;
    int location_baseAlpha              = 0;
    int location_skyColour              = 0;
    int location_texOffX                = 0;
    int location_texOffY                = 0;
    int location_glowAmount             = 0;
    int location_fogDensity             = 0;
    int location_fogGradient            = 0;
    int location_clipPlane              = 0;
    int location_clipPlaneBehind        = 0;
    int location_shadowMapFar           = 0;
    int location_toShadowMapSpaceFar    = 0;
    int location_shadowMapClose         = 0;
    int location_toShadowMapSpaceClose  = 0;
    int location_randomMap              = 0;
    int location_mixFactor              = 0;
    int location_textureSampler2        = 0;
    int location_fogScale               = 0;
    int location_fogBottomPosition      = 0;
    int location_fogBottomThickness     = 0;
    int location_depthBufferTransparent = 0;
    int location_isRenderingTransparent = 0;
    int location_waterColor             = 0;
    int location_waterBlendAmount       = 0;
    int location_waterMurkyAmount       = 0;
    int location_isRenderingDepth       = 0;

public:
    EntityShader(const char* vertexFile, const char* fragmentFile);

    void start();

    void stop();

    void cleanUp();

    void loadTransformationMatrix(Matrix4f* matrix);

    void loadProjectionMatrix(Matrix4f* matrix);

    void loadViewMatrix(Camera* camera);

    void loadSun(Light* light);

    void loadShineVariables(float damper, float reflectivity);

    void loadFakeLighting(int fakeLighting);

    void loadTransparency(int transparency);

    void loadGlowAmount(float glowAmount);

    void loadBaseColour(Vector3f* baseColour);

    void loadBaseAlpha(float baseAlpha);

    void loadTextureOffsets(float offX, float offY);

    void loadSkyColour(float r, float g, float b);

    void loadFogDensity(float density);

    void loadFogGradient(float gradient);

    void loadFogBottomPosition(float position);

    void loadFogBottomThickness(float thickness);

    void loadClipPlane(float clipX, float clipY, float clipZ, float clipW);

    void loadClipPlaneBehind(float clipX, float clipY, float clipZ, float clipW);

    void loadToShadowSpaceMatrixFar(Matrix4f* matrix);

    void loadToShadowSpaceMatrixClose(Matrix4f* matrix);

    void loadMixFactor(float factor);

    void loadFogScale(float scale);

    void loadIsRenderingTransparent(bool value);

    void loadWaterColor(Vector3f* color);

    void loadWaterBlendAmount(float);

    void loadIsRenderingDepth(bool value);

    void connectTextureUnits();

protected:
    void bindAttributes();

    void bindAttribute(int attribute, const char* variableName);

    void bindFragOutput(int attatchment, const char* variableName);

    void getAllUniformLocations();

    int getUniformLocation(const char* uniformName);

    void loadFloat(int location, float value);

    void loadInt(int location, int value);

    void loadVector(int location, Vector3f* vect);

    void loadBoolean(int location, bool value);

    void loadMatrix(int location, Matrix4f* matrix);
};

#endif
