#include "verticalblurshader.h"
#include "../renderEngine/loader.h"

#include <glad/glad.h>

VerticalBlurShader::VerticalBlurShader(const char* vFile, const char* fFile)
{
    vertexShaderId = Loader::loadShader(vFile, GL_VERTEX_SHADER);
    fragmentShaderId = Loader::loadShader(fFile, GL_FRAGMENT_SHADER);
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bindAttributes();
    glLinkProgram(programId);
    glValidateProgram(programId);
    getAllUniformLocations();
}

void VerticalBlurShader::start()
{
    glUseProgram(programId);
}

void VerticalBlurShader::stop()
{
    glUseProgram(0);
}

void VerticalBlurShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void VerticalBlurShader::loadTargetHeight(float height)
{
    loadFloat(location_targetHeight, height);
}

void VerticalBlurShader::bindAttributes()
{
    bindAttribute(0, "position");
}

void VerticalBlurShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void VerticalBlurShader::getAllUniformLocations()
{
    location_targetHeight = getUniformLocation("targetHeight");
}

int VerticalBlurShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void VerticalBlurShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}
