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
#include "particleshader.h"

ParticleShader::ParticleShader()
{
    vertexShaderId = Loader::loadShader("res/Shaders/particles/particleVShader.txt", GL_VERTEX_SHADER);
    fragmentShaderId = Loader::loadShader("res/Shaders/particles/particleFShader.txt", GL_FRAGMENT_SHADER);
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bindAttributes();
    glLinkProgram(programId);
    glValidateProgram(programId);
    getAllUniformLocations();
}

void ParticleShader::start()
{
    glUseProgram(programId);
}

void ParticleShader::stop()
{
    glUseProgram(0);
}

void ParticleShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void ParticleShader::loadBrightness(float brightness)
{
    loadFloat(location_brightness, brightness);
}

void ParticleShader::loadOpacity(float opacity)
{
    loadFloat(location_opacity, opacity);
}

void ParticleShader::loadGlow(float glow)
{
    loadFloat(location_glow, glow);
}

void ParticleShader::loadNumberOfRows(float numberRows)
{
    loadFloat(location_numberOfRows, numberRows);
}

void ParticleShader::loadProjectionMatrix(Matrix4f* projectionMatrix)
{
    loadMatrix(location_projectionMatrix, projectionMatrix);
}

void ParticleShader::bindAttributes()
{
    bindAttribute(0, "position");
    bindAttribute(1, "modelViewMatrix");
    bindAttribute(5, "textureOffsets");
    bindAttribute(6, "blendFactor");
}

void ParticleShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void ParticleShader::getAllUniformLocations()
{
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_numberOfRows     = getUniformLocation("numberOfRows");
    location_brightness       = getUniformLocation("brightness");
    location_opacity          = getUniformLocation("opacity");
    location_glow             = getUniformLocation("glow");
}

int ParticleShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void ParticleShader::loadInt(int location, int value)
{
    glUniform1i(location, value);
}

void ParticleShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}

void ParticleShader::loadVector(int location, Vector3f* vect)
{
    glUniform3f(location, vect->x, vect->y, vect->z);
}

void ParticleShader::load2DVector(int location, Vector2f* vector)
{
    glUniform2f(location, vector->x, vector->y);
}

void ParticleShader::loadBoolean(int location, float value)
{
    glUniform1f(location, round(value));
}

void ParticleShader::loadMatrix(int location, Matrix4f* matrix)
{
    float buf[16];
    matrix->store(buf);
    glUniformMatrix4fv(location, 1, GL_FALSE, buf);
}
