#include <glad/glad.h>
#include "shadowframebuffer.h"
#include "../renderEngine/renderEngine.h"

ShadowFrameBuffer::ShadowFrameBuffer(int width, int height)
{
	this->WIDTH = width;
	this->HEIGHT = height;
	initialiseFrameBuffer();
}

void ShadowFrameBuffer::cleanUp()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &shadowMap);
}

void ShadowFrameBuffer::bindFrameBuffer()
{
	bindFrameBuffer(fbo, WIDTH, HEIGHT);
}

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

void ShadowFrameBuffer::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

GLuint ShadowFrameBuffer::getShadowMap()
{
	return shadowMap;
}

void ShadowFrameBuffer::initialiseFrameBuffer()
{
	fbo = ShadowFrameBuffer::createFrameBuffer();
	shadowMap = ShadowFrameBuffer::createDepthBufferAttachment(WIDTH, HEIGHT);
	unbindFrameBuffer();
}

void ShadowFrameBuffer::bindFrameBuffer(GLuint frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

int ShadowFrameBuffer::createFrameBuffer()
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_NONE);
	return frameBuffer;
}

int ShadowFrameBuffer::createDepthBufferAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Depth bits
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

