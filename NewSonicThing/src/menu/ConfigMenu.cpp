#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "configmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
#include "../fontMeshCreator/guitext.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../entities/camera.h"
#include "../guis/guitextureresources.h"
#include "../guis/guimanager.h"
#include "../guis/guitexture.h"
#include "button.h"
#include "mainmenu.h"
#include "../toolbox/maths.h"

ConfigMenu::ConfigMenu()
{
    std::cout << "Initializing Config Menu\n";
    this->init();
    this->loadResources();
    this->visible = true;
    this->offsetCurr = 0.0f;
    this->offsetTarget = 0.0f;
    std::cout << "Config Menu initialized\n";
}

void ConfigMenu::init()
{
    fontSize = 0.05f;
    textureParallelogram = Loader::loadTextureNoInterpolation(
        "res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop = Loader::loadTextureNoInterpolation(
        "res/Images/MainMenu/ParallelogramBackdrop.png");
    textureParallelogramHalf = Loader::loadTextureNoInterpolation(
        "res/Images/MainMenu/ParallelogramHalf.png");
    textureParallelogramHalfBackdrop = Loader::loadTextureNoInterpolation(
        "res/Images/MainMenu/ParallelogramHalfBackdrop.png");
    textureParallelogramHalf2 = Loader::loadTextureNoInterpolation(
        "res/Images/MainMenu/ParallelogramHalf2.png");
    textureParallelogramHalf2Backdrop = Loader::loadTextureNoInterpolation(
        "res/Images/MainMenu/ParallelogramHalf2Backdrop.png");
}

ConfigMenu::~ConfigMenu()
{
    std::cout << "Deleting Config Menu\n";
    this->unloadResources();
    std::cout << "Config Menu deleted.\n";
}

void ConfigMenu::loadResources()
{
    if (buttonsNames.size() != 0)
    {
        std::fprintf(stdout, "Warning: ConfigMenu loading resources when they are already loaded.\n");
    }

    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;
    float aspectRatio = (float)SCR_WIDTH / SCR_HEIGHT;

    buttonsNames.clear();

    buttonsNames.push_back(new Button("Window Width",     Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Window Height",    Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Fullscreen",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(2)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Unlock FPS",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(3)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("FOV",              Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(4)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("SFX Volume",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(5)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("BGM Volume",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(6)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Render Particles", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(7)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    
    buttonsValues.clear();
    extern float VFOV_BASE;
    buttonsValues.push_back(new Button(std::to_string(SCR_WIDTH),                         Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(std::to_string(SCR_HEIGHT),                        Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(Global::useFullscreen),               Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(2)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(Global::framerateUnlock),             Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(3)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(floatToString(VFOV_BASE),                          Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(4)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(floatToString(100*AudioPlayer::soundLevelSFX)+"%", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(5)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(floatToString(100*AudioPlayer::soundLevelBGM)+"%", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(6)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(Global::renderParticles),             Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(7)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
}

std::string ConfigMenu::boolToString(bool value)
{
    if (value)
    {
        return "Yes";
    }

    return "No";
}

std::string ConfigMenu::floatToString(float value)
{
    char buf[128];
    snprintf(buf, 128, "%g", value);
    return buf;
}

void ConfigMenu::unloadResources()
{
    std::cout << "Unloading Config Menu resources.\n";
    if ((int)buttonsNames.size() == 0)
    {
        std::fprintf(stdout, "Warning: ConfigMenu unloading resources when they are empty.\n");
    }

    GuiManager::clearGuisToRender();

    for (int i = 0; i < (int)buttonsNames.size(); i++)
    {
        delete buttonsNames[i]; INCR_DEL("Button");
    }
    buttonsNames.clear();

    for (int i = 0; i < (int)buttonsValues.size(); i++)
    {
        delete buttonsValues[i]; INCR_DEL("Button");
    }
    buttonsValues.clear();

    currentButtonIndex = 0;
    std::cout << "Config Menu resources deleted.\n";
}

void ConfigMenu::draw()
{
    extern float dt;

    offsetTarget = -currentButtonIndex*0.1f;
    offsetCurr = Maths::approach(offsetCurr, offsetTarget, 15.0f, dt);
    if (fabsf(offsetTarget - offsetCurr) < 0.002f)
    {
        offsetCurr = offsetTarget;
    }

    GuiManager::clearGuisToRender();
    
    if (visible)
    {
        for (int i = 0; i < (int)buttonsNames.size(); i++)
        {
            buttonsNames[i]->setPos(0.32f, 0.5f + offsetCurr + (0.1f*(i)));
            buttonsNames[i]->setVisible(true);
            buttonsNames[i]->setHighlight(false);

            buttonsValues[i]->setPos(0.67f, 0.5f + offsetCurr + (0.1f*(i)));
            buttonsValues[i]->setVisible(true);
            buttonsValues[i]->setHighlight(false);
        }
    }

    buttonsNames[currentButtonIndex]->setHighlight(true);
    buttonsValues[currentButtonIndex]->setHighlight(true);
}

void ConfigMenu::setVisible(bool visibleStatus)
{
    for (int i = 0; i < (int)buttonsNames.size(); i++)
    {
        buttonsNames[i]->setVisible(visibleStatus);
        buttonsNames[i]->setHighlight(visibleStatus);
        buttonsValues[i]->setVisible(visibleStatus);
        buttonsValues[i]->setHighlight(visibleStatus);
    }

    visible = visibleStatus;
}

Menu* ConfigMenu::step()
{
    Menu* retVal = nullptr;

    extern float dt;
    static float holdTick;

    int holdMultiplier = 1;
    bool shouldGoDown = false;
    bool shouldGoUp = false;
    bool shouldGoLeft = false;
    bool shouldGoRight = false;
    bool pressedSelect = false;
    bool pressedBack = false;

    int moveY = (int)round(Input::inputs.INPUT_Y);
    int moveX = (int)round(Input::inputs.INPUT_X);

    setVisible(true);

    if ((Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || 
        (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START))
    {
        pressedSelect = true;
    }

    if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2)
    {
        pressedBack = true;
    }

    if (moveY != moveYPrevious)
    {
        if (moveY == 1)
        {
            shouldGoDown = true;
        }
        else if (moveY == -1)
        {
            shouldGoUp = true;
        }
    }

    moveYPrevious = moveY;

    if (moveY == 1)
    {
        holdDownTimer += dt;
    }
    else
    {
        holdDownTimer = 0.0f;
    }

    if (moveY == -1)
    {
        holdUpTimer += dt;
    }
    else
    {
        holdUpTimer = 0.0f;
    }

    if (holdDownTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.1f)
        {
            shouldGoDown = true;
            holdTick -= 0.1f;
        }
    }

    if (holdUpTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.1f)
        {
            shouldGoUp = true;
            holdTick -= 0.1f;
        }
    }

    if (moveX != moveXPrevious)
    {
        if (moveX == 1)
        {
            shouldGoRight = true;
        }
        else if (moveX == -1)
        {
            shouldGoLeft = true;
        }
    }

    moveXPrevious = moveX;

    if (moveX == 1)
    {
        holdRightTimer += dt;
    }
    else
    {
        holdRightTimer = 0.0f;
    }

    if (moveX == -1)
    {
        holdLeftTimer += dt;
    }
    else
    {
        holdLeftTimer = 0.0f;
    }

    if (holdRightTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.03f)
        {
            shouldGoRight = true;
            holdTick -= 0.03f;
            holdMultiplier = 5;
        }
    }

    if (holdLeftTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.03f)
        {
            shouldGoLeft = true;
            holdTick -= 0.03f;
            holdMultiplier = 5;
        }
    }

    if (shouldGoUp)
    {
        if (currentButtonIndex > 0)
        {
            currentButtonIndex--;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }
    if (shouldGoDown)
    {
        if (currentButtonIndex < (int)buttonsNames.size()-1)
        {
            currentButtonIndex++;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }

    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;
    extern float VFOV_BASE;

    if (shouldGoLeft)
    {
        switch (currentButtonIndex)
        {
            case 0:
                //glfwSetWindowSize(getWindow(), SCR_WIDTH - 1*holdMultiplier, SCR_HEIGHT);
                //buttonsValues[0]->generateText(std::to_string(SCR_WIDTH));
                break;

            case 1:
                //glfwSetWindowSize(getWindow(), SCR_WIDTH, SCR_HEIGHT - 1*holdMultiplier);
                //buttonsValues[1]->generateText(std::to_string(SCR_HEIGHT));
                break;

            case 2:
                break;

            case 3:
                Global::framerateUnlock = !Global::framerateUnlock;
                buttonsValues[3]->generateText(boolToString(Global::framerateUnlock));
                break;

            case 4:
                VFOV_BASE = VFOV_BASE - 1.0f;
                buttonsValues[4]->generateText(floatToString(VFOV_BASE));
                Master_makeProjectionMatrix();
                break;

            case 5:
                if (AudioPlayer::soundLevelSFX > 0.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelSFX = std::fmaxf(0.0f, AudioPlayer::soundLevelSFX - 0.01f);
                AudioPlayer::soundLevelSFX = std::roundf(100*AudioPlayer::soundLevelSFX)/100.0f;
                buttonsValues[5]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelSFX))+"%");
                break;

            case 6:
                if (AudioPlayer::soundLevelBGM > 0.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelBGM = std::fmaxf(0.0f, AudioPlayer::soundLevelBGM - 0.01f);
                AudioPlayer::soundLevelBGM = std::roundf(100*AudioPlayer::soundLevelBGM)/100.0f;
                buttonsValues[6]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelBGM))+"%");
                break;

            case 7:
                Global::renderParticles = !Global::renderParticles;
                buttonsValues[7]->generateText(boolToString(Global::renderParticles));
                break;

            default:
                break;
        }
    }

    if (shouldGoRight)
    {
        switch (currentButtonIndex)
        {
            case 0:
                //glfwSetWindowSize(getWindow(), SCR_WIDTH + 1*holdMultiplier, SCR_HEIGHT);
                //buttonsValues[0]->generateText(std::to_string(SCR_WIDTH));
                break;

            case 1:
                //glfwSetWindowSize(getWindow(), SCR_WIDTH, SCR_HEIGHT + 1*holdMultiplier);
                //buttonsValues[1]->generateText(std::to_string(SCR_HEIGHT));
                break;

            case 2:
                break;

            case 3:
                Global::framerateUnlock = !Global::framerateUnlock;
                buttonsValues[3]->generateText(boolToString(Global::framerateUnlock));
                break;

            case 4:
                VFOV_BASE = VFOV_BASE + 1.0f;
                buttonsValues[4]->generateText(floatToString(VFOV_BASE));
                Master_makeProjectionMatrix();
                break;

            case 5:
                if (AudioPlayer::soundLevelSFX < 1.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelSFX = std::fminf(1.0f, AudioPlayer::soundLevelSFX + 0.01f);
                AudioPlayer::soundLevelSFX = std::roundf(100*AudioPlayer::soundLevelSFX)/100.0f;
                buttonsValues[5]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelSFX))+"%");
                break;

            case 6:
                if (AudioPlayer::soundLevelBGM < 1.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelBGM = std::fminf(1.0f, AudioPlayer::soundLevelBGM + 0.01f);
                AudioPlayer::soundLevelBGM = std::roundf(100*AudioPlayer::soundLevelBGM)/100.0f;
                buttonsValues[6]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelBGM))+"%");
                break;

            case 7:
                Global::renderParticles = !Global::renderParticles;
                buttonsValues[7]->generateText(boolToString(Global::renderParticles));
                break;

            default:
                break;
        }
    }

    draw();

    if (pressedSelect)
    {
        switch (currentButtonIndex)
        {
            case 3:
                Global::framerateUnlock = !Global::framerateUnlock;
                buttonsValues[3]->generateText(boolToString(Global::framerateUnlock));
                break;

            case 7:
                Global::renderParticles = !Global::renderParticles;
                buttonsValues[7]->generateText(boolToString(Global::renderParticles));
                break;

            default:
                break;
        }
    }
    else if (pressedBack)
    {
        AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
        retVal = PopMenu::get();
    }

    return retVal;
}
