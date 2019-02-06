#ifndef GUINUMBER_H
#define GUINUMBER_H

class FontType;

#include <glad/glad.h>
#include <string>
#include <vector>

#include "../toolbox/vector.h"

class GUINumber
{
private:
	static void createNumber(int i, float x, float y, float w, float h);
	static FontType* numberFont;
	static GLuint numberMeshIDs[10];
	static GLuint numberMeshVertexCounts[10];
	Vector2f position;

public:
	//Call this once to load the mesh data from the file
	static void loadMeshData();

	//please consider these read only outside of the GUINumber functions
	std::vector<GLuint>   meshIDs;
	std::vector<GLsizei>  meshVertexCounts;
	std::vector<Vector2f> meshPositions;

	//value of the number you want displayed
	int displayNumber;

	//number of digits to display, padded with 0's if necessary. for no padding, set this to 0.
	int totalDigits;

	//height of the text, 1.0 being the entire screen height
	float size;

	//colour of the numbers. default is white
	Vector3f colour;

	//0 = left, 1 = center, 2 = right
	int alignment;

	//only render the numbers if visible is true
	bool visible;
	
	//number = the initial value of the number you want displayed
	//x, y = coords on the screen, (0,0) being the top left of the screen, (1,1) being bottom right
	//size = height of the text, 1.0 being the entire screen height
	//alignment = 0 = left, 1 = center, 2 = right
	//visible = is the number visible initially
	//totalDigits = number of digits to display, padded with 0's if necessary. for no padding, set this to 0.
	GUINumber(int number, float x, float y, float size, int alignment, bool visible, int totalDigits);

	//must be called after every time you change the number, or its position, or anything else
	void refresh();

	void deleteMe();

	Vector2f* getPosition();

	//x, y = coords on the screen, (0,0) being the top left of the screen, (1,1) being bottom right
	void setPosition(float x, float y);

	FontType* getFont() const;
};

#endif
