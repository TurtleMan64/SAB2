#ifndef FBO_H
#define FBO_H

#include <glad/glad.h>
#include <vector>

class Fbo
{
private:
	int width;
	int height;

	GLuint frameBuffer;

	bool multisampleAndMultiTarget;
	
	GLuint colourTexture;
	GLuint depthTexture;

	GLuint depthBuffer;
	GLuint colourBuffer;
	GLuint colourBuffer2;
	
public:
	static int NONE;
	static int DEPTH_TEXTURE;
	static int DEPTH_RENDER_BUFFER;

	Fbo(int width, int height, int depthBufferType);
	
	Fbo(int width, int height);
	
	void cleanUp();
	
	void bindFrameBuffer();
	
	void unbindFrameBuffer();
	
	//void bindToRead();
	
	GLuint getColourTexture();
	
	//int getDepthTexture();
	
	void resolveToFbo(int readBuffer, Fbo* outputFbo);
	
	//void resolveToScreen();
	
private:	
	void initialiseFrameBuffer(int type);
	
	void createFrameBuffer();
	
	void determineDrawBuffers();
	
	void createTextureAttachment();
	
	void createDepthTextureAttachment();
	
	GLuint createMultisampleColourAttatchment(int attachment);
	
	void createDepthBufferAttachment();
};
#endif