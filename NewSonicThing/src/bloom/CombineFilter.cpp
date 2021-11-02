#include <glad/glad.h>

#include "../engineTester/main.hpp"
#include "combinefilter.hpp"
#include "combineshader.hpp"
#include "../postProcessing/imagerenderer.hpp"

CombineFilter::CombineFilter()
{
    shader = new CombineShader("res/Shaders/bloom/simpleVertex.txt", "res/Shaders/bloom/combineFragment.txt"); INCR_NEW("CombineShader");
    shader->start();
    shader->connectTextureUnits();
    shader->stop();
    renderer = new ImageRenderer; INCR_NEW("ImageRenderer");
}

void CombineFilter::render(GLuint colourTexture, GLuint highlightTexture)
{
    shader->start();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colourTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, highlightTexture);
    renderer->renderQuad();
    shader->stop();
}

void CombineFilter::cleanUp()
{
    renderer->cleanUp();
    shader->cleanUp();
    delete shader; INCR_DEL("CombineShader");
    delete renderer; INCR_DEL("ImageRenderer");
}
