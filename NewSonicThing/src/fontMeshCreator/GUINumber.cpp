#include "guinumber.hpp"
#include "../engineTester/main.hpp"
#include "../loading/loadergl.hpp"
#include "../fontRendering/textmaster.hpp"
#include "textmeshdata.hpp"
#include "fonttype.hpp"
#include "../menu/pausescreen.hpp"
#include "../toolbox/maths.hpp"
#include "../renderEngine/display.hpp"

GLuint GUINumber::numberMeshIds[10];
GLuint GUINumber::numberMeshVertexCounts[10];

void GUINumber::createNumber(int i, float x, float y, float w, float h)
{
    float ratioScreen = 1.0f/Display::ASPECT_RATIO;
    float ratioText   = w/h;

    std::vector<float> positions;
    std::vector<float> texCoords;

    positions.push_back(-1*ratioScreen*ratioText); //top left
    positions.push_back( 1);
    positions.push_back( 1*ratioScreen*ratioText); //top right
    positions.push_back( 1);
    positions.push_back(-1*ratioScreen*ratioText); //bottom left
    positions.push_back(-1);

    positions.push_back( 1*ratioScreen*ratioText); //top right
    positions.push_back( 1);
    positions.push_back(-1*ratioScreen*ratioText); //bottom left
    positions.push_back(-1);
    positions.push_back( 1*ratioScreen*ratioText); //bottom right
    positions.push_back(-1);


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

    std::vector<int> vertexObjects = LoaderGL::loadToVAO(&positions, &texCoords);
    GUINumber::numberMeshIds[i] = (GLuint)vertexObjects[0];
    GUINumber::numberMeshVertexCounts[i] = 6;
}

void GUINumber::loadMeshData()
{
    //GUINumber::numberFont = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt"); INCR_NEW

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
GUINumber::GUINumber(int number, float x, float y, float size, int alignment, bool visible, int totalDigits, bool darkenPaddedDigits)
{
    colors.push_back(Vector3f(1, 1, 1));
    this->position.set(x-0.5f, y-0.5f);
    this->visible = visible;
    this->alignment = alignment;
    this->size = size;
    this->totalDigits = totalDigits;
    this->darkenPaddedDigits = darkenPaddedDigits;
    displayNumber = number;
    refresh();
    TextMaster::loadNumber(this);
}

//must be called every time you want to change the number, or its position, or anything else
void GUINumber::refresh()
{
    meshIds.clear();
    meshVertexCounts.clear();
    meshPositions.clear();
    colors.clear();

    int numChars = Maths::numDigits(displayNumber);

    if ((totalDigits > 0) && darkenPaddedDigits)
    {
        for (int i = 0; i < numChars; i++)
        {
            colors.push_back(Vector3f(1,1,1));
        }
        for (int i = numChars; i < totalDigits; i++)
        {
            colors.push_back(Vector3f(0.5f, 0.5f, 0.5f));
        }
    }
    else
    {
        if (totalDigits > 0)
        {
            for (int i = 0; i < totalDigits; i++)
            {
                colors.push_back(Vector3f(1,1,1));
            }
        }
        else
        {
            for (int i = 0; i < numChars; i++)
            {
                colors.push_back(Vector3f(1,1,1));
            }
        }
    }

    if (totalDigits > 0)
    {
        numChars = totalDigits;
    }

    int currentNumber = displayNumber;
    //size = currentNumber/1000.0f;

    float ratioScreen = 1.0f/Display::ASPECT_RATIO;
    float distanceBetweenCharacters = size*ratioScreen;

    switch (alignment)
    {
        case 2:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters - distanceBetweenCharacters/2, position.y + size/2));
                currentNumber = currentNumber/10;
            }
            break;

        case 1:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters/2 - distanceBetweenCharacters/2, position.y + size/2));
                currentNumber = currentNumber/10;
            }
            break;

        case 0:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters - distanceBetweenCharacters/2, position.y + size/2));
                currentNumber = currentNumber/10;
            }
            break;

        case 5:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters - distanceBetweenCharacters/2, position.y));
                currentNumber = currentNumber/10;
            }
            break;

        case 4:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters/2 - distanceBetweenCharacters/2, position.y));
                currentNumber = currentNumber/10;
            }
            break;

        case 3:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters - distanceBetweenCharacters/2, position.y));
                currentNumber = currentNumber/10;
            }
            break;

        case 8:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters - distanceBetweenCharacters/2, position.y - size/2));
                currentNumber = currentNumber/10;
            }
            break;

        case 7:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters/2 - distanceBetweenCharacters/2, position.y - size/2));
                currentNumber = currentNumber/10;
            }
            break;

        case 6:
            for (int i = 0; i < numChars; i++)
            {
                meshIds.push_back         (GUINumber::numberMeshIds[currentNumber % 10]);
                meshVertexCounts.push_back(GUINumber::numberMeshVertexCounts[currentNumber % 10]);
                meshPositions.push_back   (Vector2f(position.x - i*distanceBetweenCharacters + numChars*distanceBetweenCharacters - distanceBetweenCharacters/2, position.y - size/2));
                currentNumber = currentNumber/10;
            }
            break;

        default:
            std::fprintf(stderr, "GUINumber had wrong alignment value\n");
            break;
    }
}

void GUINumber::deleteMe()
{
    TextMaster::removeNumber(this);
}

FontType* GUINumber::getFont() const
{
    return Global::fontVipnagorgialla;
}

Vector2f* GUINumber::getPosition()
{
    return &position;
}

//x, y = coords on the screen, (0,0) being the top left of the screen, (1,1) being bottom right
void GUINumber::setPosition(float x, float y)
{
    position.set(x-0.5f, y-0.5f);
}
