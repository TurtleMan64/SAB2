#include "verticalblurshader.hpp"
#include "../loading/loadergl.hpp"

#include <glad/glad.h>

VerticalBlurShader::VerticalBlurShader(const char* vFile, const char* fFile)
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

void VerticalBlurShader::loadOriginalTexture(GLuint texId)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(location_originalTexture, 0); // might be able to do this just once at the beginning
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
    location_targetHeight    = getUniformLocation("targetHeight");
    location_originalTexture = getUniformLocation("originalTexture");
}

int VerticalBlurShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void VerticalBlurShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}
