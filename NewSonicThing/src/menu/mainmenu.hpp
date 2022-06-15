#ifndef MAINMENU_H
#define MAINMENU_H

class GUIText;
class Button;

#include <glad/glad.h>
#include <vector>
#include "../guis/guiimage.hpp"
#include "menu.hpp"

class MainMenu : public Menu
{
private:
    enum ButtonId { arcade, mission, extra, config, exit };

    int menuSelectionId = 0;    // Current menu selection highlighted

    // Title card text graphics
    GLuint textureParallelogram = GL_NONE; // Texture for button
    GLuint textureParallelogramBackdrop = GL_NONE; // Texture for button highlight
    GLuint textureLogo = GL_NONE; // Texture for logo

    GuiImage* logo = nullptr;

    std::vector<Button*> menuButtons;

    int buttonCount = 0;

    float aspectRatio = 1.0f;

    bool visible = false;

    // Animation
    int oldSelection = 0;
    float animationTime = 0.0f;
    bool animating = false;
    int animationDirection = 0;
    void resetAnimation();

    void unloadResources();

    void loadResources();

    void setVisible(bool visibleStatus);

public:
    MainMenu();

    ~MainMenu();

    Menu* step();

    void draw();
};

#endif
