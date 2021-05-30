#include <glad/glad.h>
#include "waterframebuffers.h"
#include "../engineTester/main.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

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
    reflectionTexture = createTextureAttachment(SCR_WIDTH, SCR_HEIGHT);
    reflectionDepthBuffer = createDepthBufferAttachment(SCR_WIDTH, SCR_HEIGHT);

    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer()
{
    refractionFrameBuffer  = createFrameBuffer();

    //Bug "fix" (refer above)
    //refractionTexture      = createTextureAttachment(Global::HQWaterRefractionWidth, Global::HQWaterRefractionHeight);
    //refractionDepthTexture = createDepthTextureAttachment(Global::HQWaterRefractionWidth, Global::HQWaterRefractionHeight);

    refractionTexture = createTextureAttachment(SCR_WIDTH, SCR_HEIGHT);
    refractionDepthTexture = createDepthTextureAttachment(SCR_WIDTH, SCR_HEIGHT);

    unbindCurrentFrameBuffer();
}

void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

GLuint WaterFrameBuffers::createFrameBuffer()
{
    GLuint frameBufferID = 0;
    glGenFramebuffers(1, &frameBufferID);
    //generate name for frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    //create the framebuffer
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //indicate that we will always render to color attachment 0
    return frameBufferID;
}

GLuint WaterFrameBuffers::createTextureAttachment(int width, int height)
{
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
    return textureID;
}

GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
{
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);  //here is the depth bits
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
    return textureID;
}

GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
{
    GLuint depthBufferID = 0;
    glGenRenderbuffers(1, &depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    return depthBufferID;
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

    bindFrameBuffer(reflectionFrameBuffer, SCR_WIDTH, SCR_HEIGHT);
}

void WaterFrameBuffers::bindRefractionFrameBuffer()
{
    //Bug "fix" (refer above)
    //bindFrameBuffer(refractionFrameBuffer, Global::HQWaterRefractionWidth, Global::HQWaterRefractionHeight);

    bindFrameBuffer(refractionFrameBuffer, SCR_WIDTH, SCR_HEIGHT);
}

void WaterFrameBuffers::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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
