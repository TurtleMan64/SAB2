#ifndef GUINUMBER_H
#define GUINUMBER_H

class FontType;

#include <glad/glad.h>
#include <string>
#include <vector>

#include "../toolbox/vector.h"

class GUINumber
{
public:
	static FontType* numberFont;
	static GLuint numberMeshIDs[10];
	static GLuint numberMeshVertexCounts[10];

	static void loadMeshData();
	static void createNumber(int i, float x, float y, float w, float h);

	static const float fontHeight; //height of the text, 1.0 being the entire screen height
	static float distanceBetweenCharacters; //Something that should only be read from. Changing this does not actually change the displayed text


	int displayNumber;

	Vector2f basePosition;
	Vector3f colour;
	int alignment; //0 = left, 1 = center, 2 = right
	bool visible;

	std::vector<GLuint>   meshIDs;
	std::vector<GLsizei>  meshVertexCounts;
	std::vector<Vector2f> meshPositions;
	

	GUINumber(int number, float x, float y, int alignment, bool visible);

	//must be called every time you want to change the number, or its position, or anything else
	void refresh();

	void deleteMe();

	void setColour(float r, float g, float b);

	Vector3f* getColour();

	Vector2f* getPosition();

	void setPosition(Vector2f* newPos);

	void setVisibility(bool newVisible);

	bool isVisible();
};

#endif