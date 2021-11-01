#include <glad/glad.h>

#include "shadowshader.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../toolbox/maths.h"
#include "../entities/light.h"
#include "../loading/loader.h"

ShadowShader::ShadowShader(const char* vertexFile, const char* fragmentFile)
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

void ShadowShader::start()
{
    glUseProgram(programId);
}

void ShadowShader::stop()
{
    glUseProgram(0);
}

void ShadowShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void ShadowShader::loadMvpMatrix(Matrix4f* matrix)
{
    loadMatrix(location_mvpMatrix, matrix);
}

void ShadowShader::bindAttributes()
{
    bindAttribute(0, "in_position");
    bindAttribute(1, "in_textureCoords");
}

void ShadowShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void ShadowShader::getAllUniformLocations()
{
    location_mvpMatrix = getUniformLocation("mvpMatrix");
}

int ShadowShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

float shadowMatrixBuffer[16];

void ShadowShader::loadMatrix(int location, Matrix4f* matrix)
{
    matrix->store(shadowMatrixBuffer);
    glUniformMatrix4fv(location, 1, GL_FALSE, shadowMatrixBuffer);
}
