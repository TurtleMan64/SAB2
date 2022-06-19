#ifndef CONFIGMENU_H
#define CONFIGMENU_H

class Button;

#include <glad/glad.h>
#include <vector>
#include <string>
#include "menu.hpp"

class ConfigMenu : public Menu
{
private:
    GLuint textureParallelogram              = GL_NONE;
    GLuint textureParallelogramBackdrop      = GL_NONE;
    GLuint textureParallelogramHalf          = GL_NONE;
    GLuint textureParallelogramHalfBackdrop  = GL_NONE;
    GLuint textureParallelogramHalf2         = GL_NONE;
    GLuint textureParallelogramHalf2Backdrop = GL_NONE;

    int moveXPrevious = 0;
    int moveYPrevious = 0;
    bool selectInputPrevious = false;
    bool backInputPrevious   = false;

    float holdDownTimer  = 0.0f;
    float holdUpTimer    = 0.0f;
    float holdLeftTimer  = 0.0f;
    float holdRightTimer = 0.0f;

    int currentButtonIndex = 0;
    std::vector<Button*> buttonsNames;
    std::vector<Button*> buttonsValues;

    bool visible = false;

    float fontSize = 0.05f;

    // Animation
    float offsetTarget = 0.0f;
    float offsetCurr   = 0.0f;

    static std::string boolToString(bool value);
    static std::string floatToString(float value);

    void loadResources();
    void unloadResources();

public:
    ConfigMenu();
    ~ConfigMenu();
    Menu* step();
    void draw();
    void setVisible(bool isVisible);
};

#endif
