#include "guinumber.h"
#include "../engineTester/main.h"
#include "../renderEngine/renderEngine.h"
#include "../fontRendering/textmaster.h"
#include "textmeshdata.h"
#include "fonttype.h"
#include "../toolbox/pausescreen.h"
#include "../toolbox/maths.h"

FontType* GUINumber::numberFont = nullptr;
GLuint    GUINumber::numberMeshIDs[10];
GLuint    GUINumber::numberMeshVertexCounts[10];

const float GUINumber::fontHeight = 0.06f; //height of the text, 1.0 being the entire screen height
float GUINumber::distanceBetweenCharacters = 0.0f; //Something that should only be read from. Changing this does not actually change the displayed text

void GUINumber::createNumber(int i, float x, float y, float w, float h)
{
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;

	float ratioScreen = ((float)SCR_HEIGHT)/SCR_WIDTH;
	float ratioText   = w/h;

	GUINumber::distanceBetweenCharacters = GUINumber::fontHeight*ratioScreen;

	std::vector<float> positions;
	std::vector<float> texCoords;

	positions.push_back(-1*GUINumber::fontHeight*ratioScreen*ratioText); //top left
	positions.push_back( 1*GUINumber::fontHeight);
	positions.push_back( 1*GUINumber::fontHeight*ratioScreen*ratioText); //top right
	positions.push_back( 1*GUINumber::fontHeight);
	positions.push_back(-1*GUINumber::fontHeight*ratioScreen*ratioText); //bottom left
	positions.push_back(-1*GUINumber::fontHeight);

	positions.push_back( 1*GUINumber::fontHeight*ratioScreen*ratioText); //top right
	positions.push_back( 1*GUINumber::fontHeight);
	positions.push_back(-1*GUINumber::fontHeight*ratioScreen*ratioText); //bottom left
	positions.push_back(-1*GUINumber::fontHeight);
	positions.push_back( 1*GUINumber::fontHeight*ratioScreen*ratioText); //bottom right
	positions.push_back(-1*GUINumber::fontHeight);


	texCoords.push_back(x);   //top left
	texCoords.push_back(y);
	texCoords.push_back(x+w); //top right
	texCoords.push_back(y);
	texCoords.push_back(x);   //bottom left
	texCoords.push_back(y+h);

	texCoords.push_back(x+w);  //top right
	texCoords.push_back(y);
	texCoords.push_back(x);    //bottom left
	texCoords.push_back(y+h);
	texCoords.push_back(x+w);  //bottom right
	texCoords.push_back(y+h);

	std::vector<int> vertexObjects = Loader::loadToVAO(&positions, &texCoords);
	GUINumber::numberMeshIDs[i] = (GLuint)vertexObjects[0];
	GUINumber::numberMeshVertexCounts[i] = 6;
}

void GUINumber::loadMeshData()
{
	GUINumber::numberFont = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt"); INCR_NEW

	const float ts = 512; //size of the image used as the font atlas

	GUINumber::createNumber(0, 0  /ts, 331/ts, 58/ts, 51/ts);
	GUINumber::createNumber(1, 478/ts, 229/ts, 33/ts, 51/ts);
	GUINumber::createNumber(2, 0  /ts, 280/ts, 58/ts, 51/ts);
	GUINumber::createNumber(3, 58 /ts, 280/ts, 58/ts, 51/ts);
	GUINumber::createNumber(4, 116/ts, 280/ts, 55/ts, 51/ts);
	GUINumber::createNumber(5, 171/ts, 280/ts, 58/ts, 51/ts);
	GUINumber::createNumber(6, 229/ts, 280/ts, 58/ts, 51/ts);
	GUINumber::createNumber(7, 287/ts, 280/ts, 53/ts, 51/ts);
	GUINumber::createNumber(8, 340/ts, 280/ts, 58/ts, 51/ts);
	GUINumber::createNumber(9, 398/ts, 280/ts, 58/ts, 51/ts);
}

//x and y are (0,0) being the top left of the screen, (1,1) being bottom right
GUINumber::GUINumber(int number, float x, float y, int alignment, bool visible)
{
	colour.set(1, 1, 1);
	this->basePosition.set(x-0.5f, y-0.5f);
	this->visible = visible;
	this->alignment = alignment;
	displayNumber = number;
	refresh();
	TextMaster::loadNumber(this);
}

//must be called every time you want to change the number, or its position, or anything else
void GUINumber::refresh()
{
	meshIDs.clear();
	meshVertexCounts.clear();
	meshPositions.clear();

	int numChars = Maths::numDigits(displayNumber);

	int currentNumber = displayNumber;

	switch (alignment)
	{
		case 0:
			for (int i = 0; i < numChars; i++)
			{
				meshIDs.push_back         (GUINumber::numberMeshIDs[currentNumber % 10]);
				meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
				meshPositions.push_back   (Vector2f(basePosition.x - i*distanceBetweenCharacters - distanceBetweenCharacters/2, basePosition.y + GUINumber::fontHeight/2));
				currentNumber = currentNumber/10;
			}
			break;

		case 1:
			for (int i = 0; i < numChars; i++)
			{
				meshIDs.push_back         (GUINumber::numberMeshIDs[currentNumber % 10]);
				meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
				meshPositions.push_back   (Vector2f(basePosition.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters/2 - distanceBetweenCharacters/2, basePosition.y + GUINumber::fontHeight/2));
				currentNumber = currentNumber/10;
			}
			break;

		default:
			for (int i = 0; i < numChars; i++)
			{
				meshIDs.push_back         (GUINumber::numberMeshIDs[currentNumber % 10]);
				meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
				meshPositions.push_back   (Vector2f(basePosition.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters - distanceBetweenCharacters/2, basePosition.y + GUINumber::fontHeight/2));
				currentNumber = currentNumber/10;
			}
			break;
	}
}

void GUINumber::deleteMe()
{
	TextMaster::removeNumber(this);
}

void GUINumber::setColour(float r, float g, float b)
{
	colour.set(r, g, b);
}

Vector3f* GUINumber::getColour()
{
	return &colour;
}

Vector2f* GUINumber::getPosition()
{
	return &basePosition;
}

void GUINumber::setPosition(Vector2f* newPos)
{
	basePosition.set(newPos);
}

void GUINumber::setVisibility(bool newVisible)
{
	visible = newVisible;
}

bool GUINumber::isVisible()
{
	return visible;
}
