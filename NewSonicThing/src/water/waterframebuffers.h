#ifndef WATERFRAMEBUFFERS_H
#define WATERFRAMEBUFFERS_H

#include <glad/glad.h>

class WaterFrameBuffers
{
private:
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

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