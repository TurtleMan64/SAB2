#ifndef CHARACTERSELECT_H
#define CHARACTERSELECT_H

class Button;

#include <glad/glad.h>
#include <vector>
#include <string>
#include "menu.hpp"
#include "../guis/guiimage.hpp"

class CharacterSelect : public Menu
{
private:
    GLuint textureParallelogram         = GL_NONE;
    GLuint textureParallelogramBackdrop = GL_NONE;
    GLuint textureSonic                 = GL_NONE;
    GLuint textureTails                 = GL_NONE;
    GLuint textureKnuckles              = GL_NONE;

    //int moveXPrevious = 0;
    int moveYPrevious = 0;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

    float holdDownTimer = 0;
    float holdUpTimer = 0;
    //float holdLeftTimer = 0;
    //float holdRightTimer = 0;

    const float separation = 0.2f;

    int currentButtonIndex = 0;
    std::vector<Button*> buttonsCharacterNames;
    std::vector<GuiImage*> characterIcons;

    bool visible = false;

    // Animation
    float offsetTarget = 0;
    float offsetCurr = 0;

private:
    void draw();
    void setVisible(bool visibleStatus);

    void loadResources();
    void unloadResources();

public:
    CharacterSelect();
    ~CharacterSelect();
    Menu* step();
    
    float fontSize;
};

#endif
