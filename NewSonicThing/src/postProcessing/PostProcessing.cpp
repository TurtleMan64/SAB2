#include <glad/glad.h>
#include <vector>

#include "../engineTester/main.hpp"
#include "postprocessing.hpp"
#include "../models/models.hpp"
#include "../gaussianBlur/horizontalblur.hpp"
#include "../gaussianBlur/verticalblur.hpp"
#include "../bloom/combinefilter.hpp"
#include "../loading/loadergl.hpp"
#include "../renderEngine/display.hpp"

std::vector<float> PostProcessing::POSITIONS;
RawModel        PostProcessing::quadModel;
HorizontalBlur* PostProcessing::hBlur  = nullptr;
VerticalBlur*   PostProcessing::vBlur  = nullptr;
HorizontalBlur* PostProcessing::hBlur2 = nullptr;
VerticalBlur*   PostProcessing::vBlur2 = nullptr;
CombineFilter*  PostProcessing::combineFilter = nullptr;

void PostProcessing::init()
{
    PostProcessing::POSITIONS.push_back(-1);
    PostProcessing::POSITIONS.push_back(1);
    PostProcessing::POSITIONS.push_back(-1);
    PostProcessing::POSITIONS.push_back(-1);
    PostProcessing::POSITIONS.push_back(1);
    PostProcessing::POSITIONS.push_back(1);
    PostProcessing::POSITIONS.push_back(1);
    PostProcessing::POSITIONS.push_back(-1);
    
    PostProcessing::quadModel = LoaderGL::loadToVAO(&PostProcessing::POSITIONS, 2);
    
    PostProcessing::hBlur  = new HorizontalBlur(Display::WINDOW_WIDTH/16, Display::WINDOW_HEIGHT/16); INCR_NEW("HorizontalBlur");
    PostProcessing::vBlur  = new VerticalBlur  (Display::WINDOW_WIDTH/16, Display::WINDOW_HEIGHT/16); INCR_NEW("VerticalBlur");
    PostProcessing::hBlur2 = new HorizontalBlur(Display::WINDOW_WIDTH/6,  Display::WINDOW_HEIGHT/6);  INCR_NEW("HorizontalBlur");
    PostProcessing::vBlur2 = new VerticalBlur  (Display::WINDOW_WIDTH/6,  Display::WINDOW_HEIGHT/6);  INCR_NEW("VerticalBlur");
    PostProcessing::combineFilter = new CombineFilter; INCR_NEW("CombineFilter");
}

void PostProcessing::doPostProcessing(int colorTexture, int brightTexture)
{
    ANALYSIS_START("Post Processing");
    PostProcessing::start();
    PostProcessing::hBlur2->render(brightTexture);
    PostProcessing::vBlur2->render(PostProcessing::hBlur2->getOutputTexture());
    PostProcessing::hBlur->render(PostProcessing::vBlur2->getOutputTexture());
    PostProcessing::vBlur->render(PostProcessing::hBlur->getOutputTexture());
    PostProcessing::combineFilter->render(colorTexture, PostProcessing::vBlur->getOutputTexture());
    PostProcessing::end();
    ANALYSIS_DONE("Post Processing");
}

void PostProcessing::cleanUp()
{
    PostProcessing::hBlur->cleanUp();
    PostProcessing::vBlur->cleanUp();
    PostProcessing::hBlur2->cleanUp();
    PostProcessing::vBlur2->cleanUp();
    PostProcessing::combineFilter->cleanUp();
}

void PostProcessing::start()
{
    glBindVertexArray(PostProcessing::quadModel.getVaoId());
    glEnableVertexAttribArray(0);
    glDisable(GL_DEPTH_TEST);
}

void PostProcessing::end()
{
    glEnable(GL_DEPTH_TEST);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
