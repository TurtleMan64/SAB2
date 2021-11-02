#include <glad/glad.h>

#include "shadowshader2.hpp"
#include "../toolbox/vector.hpp"
#include "../toolbox/matrix.hpp"
#include "../entities/camera.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/light.hpp"
#include "../loading/loader.hpp"

ShadowShader2::ShadowShader2(const char* vertexFile, const char* fragmentFile)
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

void ShadowShader2::start()
{
    glUseProgram(programId);
}

void ShadowShader2::stop()
{
    glUseProgram(0);
}

void ShadowShader2::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void ShadowShader2::loadMvpMatrix(Matrix4f* matrix)
{
    loadMatrix(location_mvpMatrix, matrix);
}

void ShadowShader2::bindAttributes()
{
    bindAttribute(0, "in_position");
    bindAttribute(1, "in_textureCoords");
}

void ShadowShader2::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void ShadowShader2::getAllUniformLocations()
{
    location_mvpMatrix = getUniformLocation("mvpMatrix");
}

int ShadowShader2::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

float shadow2MatrixBuffer[16];

void ShadowShader2::loadMatrix(int location, Matrix4f* matrix)
{
    matrix->store(shadow2MatrixBuffer);
    glUniformMatrix4fv(location, 1, GL_FALSE, shadow2MatrixBuffer);
}
