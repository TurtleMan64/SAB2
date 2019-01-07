#include <glad/glad.h>

#include "imagerenderer.h"
#include "fbo.h"
#include "../engineTester/main.h"

ImageRenderer::ImageRenderer(int width, int height)
{
	fbo = new Fbo(width, height, Fbo::NONE); INCR_NEW
}

ImageRenderer::ImageRenderer()
{
	
}

void ImageRenderer::renderQuad()
{
	if (fbo != nullptr) 
	{
		fbo->bindFrameBuffer();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo != nullptr) 
	{
		fbo->unbindFrameBuffer();
	}
}

int ImageRenderer::getOutputTexture()
{
	return fbo->getColourTexture();
}

void ImageRenderer::cleanUp()
{
	if (fbo != nullptr) 
	{
		fbo->cleanUp();
		delete fbo; INCR_DEL
	}
}
