#ifndef GUINUMBER_H
#define GUINUMBER_H

class FontType;

#include <glad/glad.h>
#include <string>
#include <vector>

#include "../toolbox/vector.hpp"

class GUINumber
{
private:
    static void createNumber(int i, float x, float y, float w, float h);
    static GLuint numberMeshIds[10];
    static GLuint numberMeshVertexCounts[10];
    Vector2f position;

public:
    //Call this once to load the mesh data from the file
    static void loadMeshData();

    //please consider these read only outside of the GUINumber functions
    std::vector<GLuint>   meshIds;
    std::vector<GLsizei>  meshVertexCounts;
    std::vector<Vector2f> meshPositions;

    //value of the number you want displayed
    int displayNumber;

    //number of digits to display, padded with 0's if necessary. for no padding, set this to 0.
    int totalDigits;

    //height of the text, 1.0 being the entire screen height
    float size;

    //if you pad with 0's, set this to darken them
    bool darkenPaddedDigits;

    //colours of the numbers. default is white
    std::vector<Vector3f> colours;

    //0 = left, 1 = center, 2 = right
    int alignment;

    //only render the numbers if visible is true
    bool visible;
    
    //number = the initial value of the number you want displayed
    //x, y = coords on the screen, (0,0) being the top left of the screen, (1,1) being bottom right
    //size = height of the text, 1.0 being the entire screen height
    //alignment = 0 = top-left, 1 = top-center, 2 = top-right, 3 = mid-left, 4 = mid-center, 5 = mid-right, 6 = bot-left, 7 = bot-center, 8 = bot-right
    //chart: 
    //  0 1 2
    //  3 4 5
    //  6 7 8
    //visible = is the number visible initially
    //totalDigits = number of digits to display, padded with 0's if necessary. for no padding, set this to 0.
    //darkenPaddedDigits = if you pad with 0's, set this to darken them
    GUINumber(int number, float x, float y, float size, int alignment, bool visible, int totalDigits, bool darkenPaddedDigits);

    //must be called after every time you change the number, or its position, or anything else
    void refresh();

    void deleteMe();

    //x, y = coords on the screen, (0,0) being the top left of the screen, (1,1) being bottom right
    void setPosition(float x, float y);

    Vector2f* getPosition();

    FontType* getFont() const;
};

#endif
