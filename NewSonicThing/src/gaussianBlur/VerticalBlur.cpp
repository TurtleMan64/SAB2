#include <glad/glad.h>

#include "verticalblur.h"
#include "verticalblurshader.h"
#include "../postProcessing/imagerenderer.h"
#include "../engineTester/main.h"

VerticalBlur::VerticalBlur(int targetFboWidth, int targetFboHeight)
{
	shader = new VerticalBlurShader("res/Shaders/gaussianBlur/verticalBlurVertex.txt", "res/Shaders/gaussianBlur/blurFragment.txt"); INCR_NEW
	shader->start();
	shader->loadTargetHeight((float)targetFboHeight);
	shader->stop();
	renderer = new ImageRenderer(targetFboWidth, targetFboHeight); INCR_NEW
}

void VerticalBlur::render(GLuint texture)
{
	shader->start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	renderer->renderQuad();
	shader->stop();
}

int VerticalBlur::getOutputTexture()
{
	return renderer->getOutputTexture();
}

void VerticalBlur::cleanUp()
{
	renderer->cleanUp();
	shader->cleanUp();
	delete shader; INCR_DEL
	delete renderer; INCR_DEL
}
