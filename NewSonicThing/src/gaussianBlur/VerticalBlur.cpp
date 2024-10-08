#include <glad/glad.h>

#include "verticalblur.hpp"
#include "verticalblurshader.hpp"
#include "../postProcessing/imagerenderer.hpp"
#include "../engineTester/main.hpp"

VerticalBlur::VerticalBlur(int targetFboWidth, int targetFboHeight)
{
    shader = new VerticalBlurShader("res/Shaders/gaussianBlur/BlurVerticalVert.glsl", "res/Shaders/gaussianBlur/BlurFrag.glsl"); INCR_NEW("VerticalBlurShader");
    shader->start();
    shader->loadTargetHeight((float)targetFboHeight);
    shader->stop();
    renderer = new ImageRenderer(targetFboWidth, targetFboHeight); INCR_NEW("ImageRenderer");
}

void VerticalBlur::render(GLuint texture)
{
    shader->start();
    shader->loadOriginalTexture(texture);
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
    delete shader; INCR_DEL("VerticalBlurShader");
    delete renderer; INCR_DEL("ImageRenderer");
}
