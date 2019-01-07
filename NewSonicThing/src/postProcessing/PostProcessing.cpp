#include <glad/glad.h>
#include <vector>

#include "postprocessing.h"
#include "../models/models.h"
#include "../gaussianBlur/horizontalblur.h"
#include "../gaussianBlur/verticalblur.h"
#include "../bloom/combinefilter.h"
#include "../renderEngine/renderEngine.h"

std::vector<float> PostProcessing::POSITIONS;
RawModel        PostProcessing::quadModel;
HorizontalBlur* PostProcessing::hBlur  = nullptr;
VerticalBlur*   PostProcessing::vBlur  = nullptr;
HorizontalBlur* PostProcessing::hBlur2 = nullptr;
VerticalBlur*   PostProcessing::vBlur2 = nullptr;
CombineFilter*  PostProcessing::combineFilter = nullptr;

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

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
	
	PostProcessing::quadModel = Loader::loadToVAO(&PostProcessing::POSITIONS, 2);
	
	PostProcessing::hBlur  = new HorizontalBlur(SCR_WIDTH/16, SCR_HEIGHT/16);
	PostProcessing::vBlur  = new VerticalBlur  (SCR_WIDTH/16, SCR_HEIGHT/16);
	PostProcessing::hBlur2 = new HorizontalBlur(SCR_WIDTH/6,  SCR_HEIGHT/6);
	PostProcessing::vBlur2 = new VerticalBlur  (SCR_WIDTH/6,  SCR_HEIGHT/6);
	PostProcessing::combineFilter = new CombineFilter;
}

void PostProcessing::doPostProcessing(int colourTexture, int brightTexture)
{
	PostProcessing::start();
	PostProcessing::hBlur2->render(brightTexture);
	PostProcessing::vBlur2->render(PostProcessing::hBlur2->getOutputTexture());
	PostProcessing::hBlur->render(PostProcessing::vBlur2->getOutputTexture());
	PostProcessing::vBlur->render(PostProcessing::hBlur->getOutputTexture());
	PostProcessing::combineFilter->render(colourTexture, PostProcessing::vBlur->getOutputTexture());
	PostProcessing::end();
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
	glBindVertexArray(PostProcessing::quadModel.getVaoID());
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

void PostProcessing::end()
{
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
