#ifndef WATERFRAMEBUFFERS_H
#define WATERFRAMEBUFFERS_H

#include <glad/glad.h>

class WaterFrameBuffers
{
private:
    GLuint reflectionFrameBuffer = GL_NONE;
    GLuint reflectionTexture     = GL_NONE;
    GLuint reflectionDepthBuffer = GL_NONE;

    GLuint refractionFrameBuffer  = GL_NONE;
    GLuint refractionTexture      = GL_NONE;
    GLuint refractionDepthTexture = GL_NONE;

    void initialiseReflectionFrameBuffer();

    void initialiseRefractionFrameBuffer();

    void bindFrameBuffer(GLuint frameBuffer, int width, int height);

    GLuint createFrameBuffer();

    GLuint createTextureAttachment(int width, int height);

    GLuint createDepthTextureAttachment(int width, int height);

    GLuint createDepthBufferAttachment(int width, int height);

public:
    WaterFrameBuffers();

    void cleanUp();

    void bindReflectionFrameBuffer();

    void bindRefractionFrameBuffer();

    void unbindCurrentFrameBuffer();

    GLuint getReflectionTexture();

    GLuint getRefractionTexture();

    GLuint getRefractionDepthTexture();
};
#endif
