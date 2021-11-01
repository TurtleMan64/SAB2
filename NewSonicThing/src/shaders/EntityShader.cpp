#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../toolbox/maths.h"
#include "../entities/light.h"
#include "../renderEngine/loader.h"
#include "../engineTester/main.h"
#include "entityshader.h"

EntityShader::EntityShader(const char* vertexFile, const char* fragmentFile)
{
    vertexShaderId = Loader::loadShader(vertexFile, GL_VERTEX_SHADER);
    fragmentShaderId = Loader::loadShader(fragmentFile, GL_FRAGMENT_SHADER);
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bindAttributes();
    glLinkProgram(programId);
    glValidateProgram(programId);
    getAllUniformLocations();
}

void EntityShader::start()
{
    glUseProgram(programId);
}

void EntityShader::stop()
{
    glUseProgram(0);
}

void EntityShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void EntityShader::loadTransformationMatrix(Matrix4f* matrix)
{
    loadMatrix(location_transformationMatrix, matrix);
}

void EntityShader::loadProjectionMatrix(Matrix4f* projection)
{
    loadMatrix(location_projectionMatrix, projection);
}

void EntityShader::loadViewMatrix(Camera* cam)
{
    Matrix4f viewMatrix;
    Maths::createViewMatrix(&viewMatrix, cam);
    loadMatrix(location_viewMatrix, &viewMatrix);
}

void EntityShader::loadSun(Light* light)
{
    Vector3f dirInv = light->direction.scaleCopy(-1);
    loadVector(location_sunDirInv, &dirInv);
    loadVector(location_sunColor, &light->color);
}

void EntityShader::loadShineVariables(float damper, float reflectivity)
{
    loadFloat(location_shineDamper, damper);
    loadFloat(location_reflectivity, reflectivity);
}

void EntityShader::loadFakeLighting(int fakeLighting)
{
    loadFloat(location_useFakeLighting, (float)fakeLighting);
}

void EntityShader::loadTransparency(int transparency)
{
    loadInt(location_hasTransparency, transparency);
}

void EntityShader::loadGlowAmount(float glowAmount)
{
    loadFloat(location_glowAmount, glowAmount);
}

void EntityShader::loadBaseColour(Vector3f* baseColour)
{
    loadVector(location_baseColour, baseColour);
}

void EntityShader::loadBaseAlpha(float baseAlpha)
{
    loadFloat(location_baseAlpha, baseAlpha);
}

void EntityShader::loadTextureOffsets(float offX, float offY)
{
    loadFloat(location_texOffX, offX);
    loadFloat(location_texOffY, offY);
}

void EntityShader::loadSkyColour(float r, float g, float b)
{
    Vector3f newColour(r, g, b);
    loadVector(location_skyColour, &newColour);
}

void EntityShader::loadFogDensity(float density)
{
    loadFloat(location_fogDensity, density);
}

void EntityShader::loadFogGradient(float gradient)
{
    loadFloat(location_fogGradient, gradient);
}

void EntityShader::loadFogBottomPosition(float position)
{
    loadFloat(location_fogBottomPosition, position);
}

void EntityShader::loadFogBottomThickness(float thickness)
{
    loadFloat(location_fogBottomThickness, 1/thickness);
}

void EntityShader::bindAttributes()
{
    if (Global::renderBloom)
    {
        bindFragOutput(0, "out_Colour");
        bindFragOutput(1, "out_BrightColour");
    }

    bindAttribute(0, "position");
    bindAttribute(1, "textureCoords");
    bindAttribute(2, "normal");
    bindAttribute(3, "vertexColor");
}

void EntityShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void EntityShader::bindFragOutput(int attatchment, const char* variableName)
{
    glBindFragDataLocation(programId, attatchment, variableName);
}

void EntityShader::getAllUniformLocations()
{
    location_transformationMatrix   = getUniformLocation("transformationMatrix");
    location_projectionMatrix       = getUniformLocation("projectionMatrix");
    location_viewMatrix             = getUniformLocation("viewMatrix");
    location_sunDirInv              = getUniformLocation("sunDirInv");
    location_sunColor               = getUniformLocation("sunColor");
    location_shineDamper            = getUniformLocation("shineDamper");
    location_reflectivity           = getUniformLocation("reflectivity");
    location_useFakeLighting        = getUniformLocation("useFakeLighting");
    location_hasTransparency        = getUniformLocation("hasTransparency");
    location_glowAmount             = getUniformLocation("glowAmount");
    location_baseColour             = getUniformLocation("baseColour");
    location_baseAlpha              = getUniformLocation("baseAlpha");
    location_texOffX                = getUniformLocation("texOffX");
    location_texOffY                = getUniformLocation("texOffY");
    location_skyColour              = getUniformLocation("skyColour");
    location_fogDensity             = getUniformLocation("fogDensity");
    location_fogGradient            = getUniformLocation("fogGradient");
    location_clipPlane              = getUniformLocation("clipPlane");
    location_clipPlaneBehind        = getUniformLocation("clipPlaneBehind");
    location_shadowMapFar           = getUniformLocation("shadowMapFar");
    location_toShadowMapSpaceFar    = getUniformLocation("toShadowMapSpaceFar");
    location_shadowMapClose         = getUniformLocation("shadowMapClose");
    location_toShadowMapSpaceClose  = getUniformLocation("toShadowMapSpaceClose");
    location_randomMap              = getUniformLocation("randomMap");
    location_mixFactor              = getUniformLocation("mixFactor");
    location_textureSampler2        = getUniformLocation("textureSampler2");
    location_fogScale               = getUniformLocation("fogScale");
    location_fogBottomPosition      = getUniformLocation("fogBottomPosition");
    location_fogBottomThickness     = getUniformLocation("fogBottomThickness");
    location_depthBufferTransparent = getUniformLocation("depthBufferTransparent");
    location_isRenderingTransparent = getUniformLocation("isRenderingTransparent");
    location_waterColor             = getUniformLocation("waterColor");
    location_waterBlendAmount       = getUniformLocation("waterBlendAmount");
    location_waterMurkyAmount       = getUniformLocation("waterMurkyAmount");
    location_isRenderingDepth       = getUniformLocation("isRenderingDepth");
}

int EntityShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void EntityShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}

void EntityShader::loadInt(int location, int value)
{
    glUniform1i(location, value);
}

void EntityShader::loadVector(int location, Vector3f* vect)
{
    glUniform3f(location, vect->x, vect->y, vect->z);
}

void EntityShader::loadBoolean(int location, bool value)
{
    glUniform1i(location, (int)(value));
}

void EntityShader::loadMatrix(int location, Matrix4f* matrix)
{
    float buf[16];
    matrix->store(buf);
    glUniformMatrix4fv(location, 1, GL_FALSE, buf);
}

void EntityShader::loadClipPlane(float clipX, float clipY, float clipZ, float clipW)
{
    glUniform4f(location_clipPlane, clipX, clipY, clipZ, clipW);
}

void EntityShader::loadClipPlaneBehind(float clipX, float clipY, float clipZ, float clipW)
{
    glUniform4f(location_clipPlaneBehind, clipX, clipY, clipZ, clipW);
}

void EntityShader::connectTextureUnits()
{
    if (Global::renderShadowsFar || Global::renderShadowsClose)
    {
        if (Global::renderShadowsFar)
        {
            loadInt(location_shadowMapFar, 5);
        }
        if (Global::renderShadowsClose)
        {
            loadInt(location_shadowMapClose, 6);
        }

        loadInt(location_randomMap, 7);
    }

    loadInt(location_textureSampler2, 1);
    loadInt(location_depthBufferTransparent, 8);
}

void EntityShader::loadToShadowSpaceMatrixFar(Matrix4f* matrix)
{
    loadMatrix(location_toShadowMapSpaceFar, matrix);
}

void EntityShader::loadToShadowSpaceMatrixClose(Matrix4f* matrix)
{
    loadMatrix(location_toShadowMapSpaceClose, matrix);
}

void EntityShader::loadMixFactor(float factor)
{
    loadFloat(location_mixFactor, factor);
}

void EntityShader::loadFogScale(float scale)
{
    loadFloat(location_fogScale, scale);
}

void EntityShader::loadIsRenderingTransparent(bool value)
{
    loadInt(location_isRenderingTransparent, (int)value);
}

void EntityShader::loadIsRenderingDepth(bool value)
{
    loadInt(location_isRenderingDepth, (int)value);
}

void EntityShader::loadWaterColor(Vector3f* color)
{
    loadVector(location_waterColor, color);
}

void EntityShader::loadWaterBlendAmount(float amount)
{
    loadFloat(location_waterBlendAmount, amount);
}