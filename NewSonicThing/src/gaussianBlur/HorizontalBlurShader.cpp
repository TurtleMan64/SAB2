#include "horizontalblurshader.hpp"
#include "../loading/loader.hpp"

#include <glad/glad.h>

HorizontalBlurShader::HorizontalBlurShader(const char* vFile, const char* fFile)
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

void HorizontalBlurShader::start()
{
    glUseProgram(programId);
}

void HorizontalBlurShader::stop()
{
    glUseProgram(0);
}

void HorizontalBlurShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}

void HorizontalBlurShader::loadTargetWidth(float width)
{
    loadFloat(location_targetWidth, width);
}

void HorizontalBlurShader::bindAttributes()
{
    bindAttribute(0, "position");
}

void HorizontalBlurShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void HorizontalBlurShader::getAllUniformLocations()
{
    location_targetWidth = getUniformLocation("targetWidth");
}

int HorizontalBlurShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void HorizontalBlurShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}
