#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "../engineTester/main.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../toolbox/maths.h"
#include "../entities/light.h"
#include "../renderEngine/renderEngine.h"
#include "watershader.h"

WaterShader::WaterShader()
{
    if (Global::renderShadowsFar)
    {
        vertexShaderID = Loader::loadShader("res/Shaders/water/waterVertexWithShadows.txt", GL_VERTEX_SHADER);
        fragmentShaderID = Loader::loadShader("res/Shaders/water/waterFragmentWithShadows.txt", GL_FRAGMENT_SHADER);
    }
    else
    {
        vertexShaderID = Loader::loadShader("res/Shaders/water/waterVertex.txt", GL_VERTEX_SHADER);
        fragmentShaderID = Loader::loadShader("res/Shaders/water/waterFragment.txt", GL_FRAGMENT_SHADER);
    }
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    bindAttributes();
    glLinkProgram(programID);
    glValidateProgram(programID);
    getAllUniformLocations();
}

void WaterShader::start()
{
    glUseProgram(programID);
}

void WaterShader::stop()
{
    glUseProgram(0);
}

void WaterShader::cleanUp()
{
    stop();
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(programID);
}

void WaterShader::connectTextureUnits()
{
    loadInt(location_reflectionTexture, 0);
    loadInt(location_refractionTexture, 1);
    loadInt(location_dudvMap, 2);
    loadInt(location_normalMap, 3);
    loadInt(location_depthMap, 4);
    loadInt(location_shadowMap, 5);
}

void WaterShader::loadSun(Light* sun)
{
    loadVector(location_sunColor,     &sun->color);
    loadVector(location_sunDirection, &sun->direction);
}

void WaterShader::loadWaterHeight(float waterHeight)
{
    loadFloat(location_waterHeight, waterHeight);
}

void WaterShader::loadMoveFactor(float factor)
{
    loadFloat(location_moveFactor, factor);
}

void WaterShader::loadProjectionMatrix(Matrix4f* projection)
{
    loadMatrix(location_projectionMatrix, projection);
}

void WaterShader::loadClipPlaneBehind(Vector4f* plane)
{
    loadVector4f(location_clipPlaneBehind, plane);
}

void WaterShader::loadViewMatrix(Camera* cam)
{
    Matrix4f viewMatrix;
    Maths::createViewMatrix(&viewMatrix, cam);
    loadMatrix(location_viewMatrix, &viewMatrix);
    loadVector(location_cameraPosition, &cam->eye);
}

void WaterShader::loadModelMatrix(Matrix4f* modelMatrix)
{
    loadMatrix(location_modelMatrix, modelMatrix);
}

void WaterShader::loadToShadowSpaceMatrix(Matrix4f* matrix)
{
    loadMatrix(location_toShadowMapSpace, matrix);
}

void WaterShader::bindAttributes()
{
    bindFragOutput(0, "out_Colour");
    bindFragOutput(1, "out_BrightColour");
    bindAttribute(0, "position");
}

void WaterShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programID, attribute, variableName);
}

void WaterShader::bindFragOutput(int attatchment, const char* variableName)
{
    glBindFragDataLocation(programID, attatchment, variableName);
}

void WaterShader::getAllUniformLocations()
{
    location_projectionMatrix  = getUniformLocation("projectionMatrix");
    location_viewMatrix        = getUniformLocation("viewMatrix");
    location_modelMatrix       = getUniformLocation("modelMatrix");
    location_reflectionTexture = getUniformLocation("reflectionTexture");
    location_refractionTexture = getUniformLocation("refractionTexture");
    location_dudvMap           = getUniformLocation("dudvMap");
    location_moveFactor        = getUniformLocation("moveFactor");
    location_cameraPosition    = getUniformLocation("cameraPosition");
    location_normalMap         = getUniformLocation("normalMap");
    location_sunColor          = getUniformLocation("sunColor");
    location_sunDirection      = getUniformLocation("sunDirection");
    location_depthMap          = getUniformLocation("depthMap");
    location_shadowMap         = getUniformLocation("shadowMap");
    location_toShadowMapSpace  = getUniformLocation("toShadowMapSpace");
    location_waterHeight       = getUniformLocation("waterHeight");
    location_clipPlaneBehind   = getUniformLocation("clipPlaneBehind");
}

int WaterShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programID, uniformName);
}

void WaterShader::loadInt(int location, int value)
{
    glUniform1i(location, value);
}

void WaterShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}

void WaterShader::loadVector(int location, Vector3f* vect)
{
    glUniform3f(location, vect->x, vect->y, vect->z);
}

void WaterShader::loadVector4f(int location, Vector4f* vect)
{
    glUniform4f(location, vect->x, vect->y, vect->z, vect->w);
}

void WaterShader::loadBoolean(int location, float value)
{
    glUniform1f(location, round(value));
}

void WaterShader::loadMatrix(int location, Matrix4f* matrix)
{
    matrix->store(matrixBuffer);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrixBuffer);
}
