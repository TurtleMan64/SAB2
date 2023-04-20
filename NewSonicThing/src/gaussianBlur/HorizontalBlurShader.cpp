#include "horizontalblurshader.hpp"
#include "../loading/loadergl.hpp"

#include <glad/glad.h>

HorizontalBlurShader::HorizontalBlurShader(const char* vFile, const char* fFile)
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

void HorizontalBlurShader::loadOriginalTexture(GLuint texId)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(location_originalTexture, 0); // might be able to do this just once at the beginning
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
    location_targetWidth     = getUniformLocation("targetWidth");
    location_originalTexture = getUniformLocation("originalTexture");
}

int HorizontalBlurShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

void HorizontalBlurShader::loadFloat(int location, float value)
{
    glUniform1f(location, value);
}
