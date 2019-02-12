#include <glad/glad.h>
#include <list>
#include "fontrenderer.h"
#include "../fontMeshCreator/fonttype.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/guinumber.h"
#include "fontshader.h"
#include "../engineTester/main.h"


FontRenderer::FontRenderer()
{
	shader = new FontShader("res/Shaders/fontRendering/fontVertex.txt", "res/Shaders/fontRendering/fontFragment.txt"); INCR_NEW
}


void FontRenderer::render(
	std::unordered_map<FontType*, std::list<GUIText*>>* texts, 
	std::unordered_map<FontType*, std::list<GUINumber*>>* numbers)
{
	prepare();
	for (auto kv : (*texts))
	{
		FontType* font = kv.first;
		std::list<GUIText*> listOfTexts = kv.second;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
		for (GUIText* text : listOfTexts)
		{
			if (text->isVisible())
			{
				renderText(text);
			}
		}
	}
	for (auto kv : (*numbers))
	{
		FontType* font = kv.first;
		std::list<GUINumber*> listOfNumbers = kv.second;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font->getTextureAtlas());
		for (GUINumber* number : listOfNumbers)
		{
			if (number->visible)
			{
				renderNumber(number);
			}
		}
	}
	endRendering();
}

void FontRenderer::cleanUp()
{
	shader->cleanUp();
}

void FontRenderer::prepare()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	shader->start();
}

void FontRenderer::renderText(GUIText* text)
{
	glBindVertexArray(text->getMesh());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//shader->loadScale(text->getFontSize());
	shader->loadScale(1.0f);
	shader->loadColour(text->getColour());
	shader->loadTranslation(text->getPosition());
	glDrawArrays(GL_TRIANGLES, 0, text->getVertexCount());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void FontRenderer::renderNumber(GUINumber* number)
{
	int numChars = (int)number->meshIDs.size();
	shader->loadScale(number->size);
	for (int i = 0; i < numChars; i++)
	{
		glBindVertexArray(number->meshIDs[i]);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		shader->loadColour(&number->colour);
		shader->loadTranslation(&number->meshPositions[i]);
		glDrawArrays(GL_TRIANGLES, 0, number->meshVertexCounts[i]);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void FontRenderer::endRendering()
{
	shader->stop();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
