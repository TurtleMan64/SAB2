#include <glad/glad.h>

#include "imagerenderer.hpp"
#include "fbo.hpp"
#include "../engineTester/main.hpp"

ImageRenderer::ImageRenderer(int width, int height)
{
    fbo = new Fbo(width, height, Fbo::NONE); INCR_NEW("Fbo");
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
    return fbo->getColorTexture();
}

void ImageRenderer::resize(int newWidth, int newHeight)
{
    if (fbo != nullptr)
    {
        fbo->resize(newWidth, newHeight);
    }
}

void ImageRenderer::cleanUp()
{
    if (fbo != nullptr) 
    {
        fbo->cleanUp();
        delete fbo; INCR_DEL("Fbo");
    }
}
