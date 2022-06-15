#ifndef GUITEXTURE_H
#define GUITEXTURE_H

#include <glad/glad.h>
#include "../toolbox/vector.hpp"

class GuiImage
{
private:
    Vector2f position;
    Vector2f sizeRaw;
    Vector2f sizeScaled;
    float scale = 1.0f;

public:
    GLuint textureId = GL_NONE;
    bool visible = false;
    float rotation = 0.0f;
    float alpha = 1.0f;

    GuiImage();

    //position is (0,0) = top left of screen, (1,1) is bottom right
    //Note that the image is drawn with its CENTER at position
    GuiImage(GLuint textureId, Vector2f* position, Vector2f* size, float rotation);

    //position is (0,0) = top left of screen, (1,1) is bottom right
    //Note that the image is drawn with its CENTER at position
    GuiImage(GLuint textureId, float posX, float posY, float sizeX, float sizeY, float rotation);

    ~GuiImage();

    //WARNING: ONLY use this to read the position, do NOT use this to write to the position, use setX and setY
    Vector2f* getPosition();

    Vector2f* getSizeRaw();

    Vector2f* getSizeScaled();

    //If you REALLY want control over the size that the image will actually be rendered, set that here.
    //Note that changes here will be overwritten if you call setScale() after.
    void setSizeScaled(float scaleX, float scaleY);

    //position is (0,0) = top left of screen, (1,1) is bottom right
    //Note that the image is drawn with its CENTER at position
    void setX(float newX);

    //position is (0,0) = top left of screen, (1,1) is bottom right
    //Note that the image is drawn with its CENTER at position
    void setY(float newY);

    float getScale();

    void setScale(float newScale);
};

#endif
