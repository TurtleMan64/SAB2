#include <glad/glad.h>

#include "fbo.hpp"
#include "../renderEngine/display.hpp"
#include "../renderEngine/masterrenderer.hpp"

int Fbo::NONE = 0;
int Fbo::DEPTH_TEXTURE = 1;
int Fbo::DEPTH_RENDER_BUFFER = 2;

Fbo::Fbo(int width, int height, int depthBufferType)
{
    this->width = width;
    this->height = height;
    this->multisampleAndMultiTarget = false;
    this->depthBufferType = depthBufferType;
    initialiseFrameBuffer(depthBufferType);
}

Fbo::Fbo(int width, int height)
{
    this->width = width;
    this->height = height;
    this->depthBufferType = DEPTH_RENDER_BUFFER;
    this->multisampleAndMultiTarget = true;
    initialiseFrameBuffer(DEPTH_RENDER_BUFFER);
}

void Fbo::cleanUp()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &colourTexture);
    glDeleteTextures(1, &depthTexture);
    glDeleteRenderbuffers(1, &depthBuffer);
    glDeleteRenderbuffers(1, &colourBuffer);
    glDeleteRenderbuffers(1, &colourBuffer2);
}

void Fbo::resize(int newWidth, int newHeight)
{
    cleanUp();
    width = newWidth;
    height = newHeight;
    initialiseFrameBuffer(depthBufferType);
}

void Fbo::bindFrameBuffer()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    MasterRenderer::currentBoundFramebuffer = frameBuffer;
    glViewport(0, 0, width, height);
}

void Fbo::unbindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    MasterRenderer::currentBoundFramebuffer = 0;
    glViewport(0, 0, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);
}

GLuint Fbo::getColourTexture()
{
    return colourTexture;
}

void Fbo::resolveToFbo(int readBuffer, Fbo* outputFbo)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, outputFbo->frameBuffer);
    MasterRenderer::currentBoundFramebuffer = outputFbo->frameBuffer;
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frameBuffer);
    glReadBuffer(readBuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, outputFbo->width, outputFbo->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    this->unbindFrameBuffer();
}

void Fbo::initialiseFrameBuffer(int type)
{
    createFrameBuffer();
    if (multisampleAndMultiTarget)
    {
        colourBuffer = createMultisampleColourAttatchment(GL_COLOR_ATTACHMENT0);
        colourBuffer2 = createMultisampleColourAttatchment(GL_COLOR_ATTACHMENT1);
    }
    else
    {
        createTextureAttachment();
    }
    
    if (type == DEPTH_RENDER_BUFFER) 
    {
        createDepthBufferAttachment();
    } 
    else if (type == DEPTH_TEXTURE)
    {
        createDepthTextureAttachment();
    }
    unbindFrameBuffer();
}

void Fbo::createFrameBuffer()
{
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    MasterRenderer::currentBoundFramebuffer = frameBuffer;
    determineDrawBuffers();
}

void Fbo::determineDrawBuffers()
{
    std::vector<GLenum> drawBuffers;

    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);

    if (this->multisampleAndMultiTarget)
    {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT1);
    }

    glDrawBuffers((int)drawBuffers.size(), &drawBuffers[0]);
}

void Fbo::createTextureAttachment()
{
    glGenTextures(1, &colourTexture);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);
}

void Fbo::createDepthTextureAttachment()
{
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); //depth bits
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

GLuint Fbo::createMultisampleColourAttatchment(int attachment)
{
    glGenRenderbuffers(1, &colourBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colourBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, Display::AA_SAMPLES, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, colourBuffer);
    return colourBuffer;
}

void Fbo::createDepthBufferAttachment()
{
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    
    if (!multisampleAndMultiTarget)
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); //depth bits
    }
    else
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, Display::AA_SAMPLES, GL_DEPTH_COMPONENT24, width, height);  //depth bits
    }
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}
