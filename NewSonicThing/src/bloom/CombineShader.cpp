#include "combineshader.hpp"
#include "../loading/loadergl.hpp"

#include <glad/glad.h>

CombineShader::CombineShader(const char* vFile, const char* fFile)
{
    vertexShaderId = LoaderGL::loadShader(vFile, GL_VERTEX_SHADER);
    fragmentShaderId = LoaderGL::loadShader(fFile, GL_FRAGMENT_SHADER);
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bindAttributes();
    glLinkProgram(programId);
    glValidateProgram(programId);
    getAllUniformLocations();
}

void CombineShader::start()
{
    glUseProgram(programId);
}

void CombineShader::stop()
{
    glUseProgram(0);
}

void CombineShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void CombineShader::connectTextureUnits()
{
    loadInt(location_colorTexture, 0);
    loadInt(location_highlightTexture, 1);
}

void CombineShader::bindAttributes()
{
    bindAttribute(0, "position");
}

void CombineShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void CombineShader::getAllUniformLocations()
{
    location_colorTexture = getUniformLocation("colorTexture");
    location_highlightTexture = getUniformLocation("highlightTexture");
}

int CombineShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void CombineShader::loadInt(int location, int value)
{
    glUniform1i(location, value);
}
