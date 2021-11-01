#include <glad/glad.h>
#include "waterframebuffers.h"
#include "../engineTester/main.h"
#include "../renderEngine/display.h"
#include "../renderEngine/masterrenderer.h"

WaterFrameBuffers::WaterFrameBuffers()
{
    initialiseReflectionFrameBuffer();
    initialiseRefractionFrameBuffer();
}

void WaterFrameBuffers::initialiseReflectionFrameBuffer()
{
    reflectionFrameBuffer = createFrameBuffer();

    //reflectionTexture     = createTextureAttachment(Global::HQWaterReflectionWidth, Global::HQWaterReflectionHeight);
    //reflectionDepthBuffer = createDepthBufferAttachment(Global::HQWaterReflectionWidth, Global::HQWaterReflectionHeight);

    //Curently a bug if the water texture sizes are different than window size. Bug makes refracted and reflected 
    // particles to be displaced.
    reflectionTexture = createTextureAttachment(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);
    reflectionDepthBuffer = createDepthBufferAttachment(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);

    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer()
{
    refractionFrameBuffer  = createFrameBuffer();

    //Bug "fix" (refer above)
    //refractionTexture      = createTextureAttachment(Global::HQWaterRefractionWidth, Global::HQWaterRefractionHeight);
    //refractionDepthTexture = createDepthTextureAttachment(Global::HQWaterRefractionWidth, Global::HQWaterRefractionHeight);

    refractionTexture = createTextureAttachment(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);
    refractionDepthTexture = createDepthTextureAttachment(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);

    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    MasterRenderer::currentBoundFramebuffer = frameBuffer;
    glViewport(0, 0, width, height);
}

GLuint WaterFrameBuffers::createFrameBuffer()
{
    GLuint frameBufferId = 0;
    glGenFramebuffers(1, &frameBufferId);
    //generate name for frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    MasterRenderer::currentBoundFramebuffer = frameBufferId;
    //create the framebuffer
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //indicate that we will always render to color attachment 0
    return frameBufferId;
}

GLuint WaterFrameBuffers::createTextureAttachment(int width, int height)
{
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);
    return textureId;
}

GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
{
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);  //here is the depth bits
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
    return textureId;
}

GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
{
    GLuint depthBufferId = 0;
    glGenRenderbuffers(1, &depthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);
    return depthBufferId;
}

void WaterFrameBuffers::cleanUp()
{
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}

void WaterFrameBuffers::bindReflectionFrameBuffer()
{
    //Bug "fix" (refer above)
    //bindFrameBuffer(reflectionFrameBuffer, Global::HQWaterReflectionWidth, Global::HQWaterReflectionHeight);

    bindFrameBuffer(reflectionFrameBuffer, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);
}

void WaterFrameBuffers::bindRefractionFrameBuffer()
{
    //Bug "fix" (refer above)
    //bindFrameBuffer(refractionFrameBuffer, Global::HQWaterRefractionWidth, Global::HQWaterRefractionHeight);

    bindFrameBuffer(refractionFrameBuffer, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    MasterRenderer::currentBoundFramebuffer = 0;
    glViewport(0, 0, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);
}

GLuint WaterFrameBuffers::getReflectionTexture()
{
    return reflectionTexture;
}

GLuint WaterFrameBuffers::getRefractionTexture()
{
    return refractionTexture;
}

GLuint WaterFrameBuffers::getRefractionDepthTexture()
{
    return refractionDepthTexture;
}
