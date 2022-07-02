#include <glad/glad.h>

#include "horizontalblur.hpp"
#include "horizontalblurshader.hpp"
#include "../postProcessing/imagerenderer.hpp"
#include "../engineTester/main.hpp"

HorizontalBlur::HorizontalBlur(int targetFboWidth, int targetFboHeight)
{
    shader = new HorizontalBlurShader("res/Shaders/gaussianBlur/BlurHorizontalVert.glsl", "res/Shaders/gaussianBlur/BlurFrag.glsl"); INCR_NEW("HorizontalBlurShader");
    shader->start();
    shader->loadTargetWidth((float)targetFboWidth);
    shader->stop();
    renderer = new ImageRenderer(targetFboWidth, targetFboHeight); INCR_NEW("ImageRenderer");
}

void HorizontalBlur::render(GLuint texture)
{
    shader->start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer->renderQuad();
    shader->stop();
}

int HorizontalBlur::getOutputTexture()
{
    return renderer->getOutputTexture();
}

void HorizontalBlur::cleanUp()
{
    renderer->cleanUp();
    shader->cleanUp();
    delete shader; INCR_DEL("HorizontalBlurShader");
    delete renderer; INCR_DEL("ImageRenderer");
}
