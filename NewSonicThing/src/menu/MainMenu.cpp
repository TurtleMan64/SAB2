#include "mainmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <algorithm>
#include <iostream>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
#include "button.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particletexture.h"
#include "../entities/camera.h"
#include "../particles/particlemaster.h"
#include "../guis/guitextureresources.h"
#include "../guis/guimanager.h"
#include "../guis/guitexture.h"
#include "missionmenu.h"
#include "configmenu.h"
#include "extras.h"
#include "characterselect.h"

MainMenu::MainMenu()
{
    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;

    aspectRatio = (float) SCR_WIDTH / SCR_HEIGHT;
    buttonCount = 0;
    menuSelectionID = 0;
    textureParallelogram = GL_NONE;
    textureParallelogramBackdrop = GL_NONE;
    textureLogo = GL_NONE;
    loadResources();
    setVisible(false);
}

MainMenu::~MainMenu()
{
    unloadResources();
}

void MainMenu::loadResources()
{
    //printf("Loading MainMenu resources\n");
    if (menuButtons.size() != 0)
    {
        std::fprintf(stdout, "Main Menu trying to load resources when they are already loaded!\n");
        return;
    }

    textureParallelogram = Loader::loadTexture("res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop = Loader::loadTexture("res/Images/MainMenu/ParallelogramBackdrop.png");
    textureLogo = Loader::loadTexture("res/Images/MainMenu/Logo.png");
    logo = GuiTexture(textureLogo, 0.5f, 0.4f, 0.6f * (767.0f / 784.0f) / aspectRatio, 0.6f, 0.0f);

    float fontScale = 0.08f;

    // Adding a string here *should* add a button.
    std::vector<std::string> buttonNames(
        {
        "ARCADE",
        "MISSION",
        "EXTRA",
        "CONFIG",
        "EXIT"
        }
    );

    int numButtonsToMake = (int) buttonNames.size();

    menuButtons.clear();

    for (int i = 0; i < numButtonsToMake; i++)
    {
        menuButtons.push_back(new Button(buttonNames[i], Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop,
            0.5f + (0.5f * (i - menuSelectionID) / aspectRatio), 0.8f, (fontScale * 8.0f / aspectRatio), fontScale, true)); INCR_NEW("Button");
    }

    buttonCount = numButtonsToMake;

    //printf("MainMenu Resources loaded\n");
}

void MainMenu::draw()
{
    extern float dt;

    if (oldSelection != menuSelectionID)
    {
        animationTime = 0.25f;
        animating = true;
        if (oldSelection < menuSelectionID)
        {
            animationDirection = 1;
        }
        else
        {
            animationDirection = -1;
        }
    }

    if (animating)
    {
        animationTime -= dt;
    }

    if (animationTime < 0.0f)
    {
        animationTime = 0.00f;
        animating = false;
    }

    //GuiManager::clearGuisToRender();

    if (visible)
    {
        GuiManager::addGuiToRender(&logo);

        float animationOffset = animationDirection * 12 * (animationTime) * (animationTime);

        for (int i = 0; i < buttonCount; i++)
        {
            menuButtons[i]->setPos(0.5f + (0.75f * (i - menuSelectionID) / aspectRatio) + animationOffset / aspectRatio, 0.8f);
            menuButtons[i]->setVisible(true);
            menuButtons[i]->setHighlight(false);
        }

        menuButtons[menuSelectionID]->setHighlight(true);
    }

    oldSelection = menuSelectionID;
}

void MainMenu::setVisible(bool visibleStatus)
{
    for (int i = 0; i < this->buttonCount; i++)
    {
        this->menuButtons[i]->setVisible(visibleStatus);
    }
    this->visible = visibleStatus;
}

void MainMenu::unloadResources()
{
    //std::cout << "Unloading Main Menu resources...\n";
    if (menuButtons.size() == 0)
    {
        std::fprintf(stdout, "Main Menu trying to unload resources when they are already unloaded!\n");
        return;
    }

    int numToDelete = buttonCount;

    for (int i = 0; i < numToDelete; i++)
    {
        delete menuButtons[i]; INCR_DEL("Button");
    }

    menuButtons.clear();

    Loader::deleteTexture(textureParallelogram);
    Loader::deleteTexture(textureParallelogramBackdrop);
    Loader::deleteTexture(textureLogo);
    textureParallelogram         = GL_NONE;
    textureParallelogramBackdrop = GL_NONE;
    textureLogo                  = GL_NONE;
    logo.setTexture(GL_NONE);

    //GuiManager::clearGuisToRender();
    //std::cout << "Main Menu resources unloaded!\n";
}

void MainMenu::resetAnimation()
{
    animating = false;
    animationTime = 0;
}

Menu* MainMenu::step()
{
    Menu* retVal = nullptr;

    setVisible(true);

    //bool shouldGoUp    = false;
    //bool shouldGoDown  = false;
    //bool shouldGoLeft  = false;
    //bool shouldGoRight = false;
    bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
    //bool pressedBack   = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);

    static int xPrev;
    //static int yPrev;

    int moveX = (int)round(Input::inputs.INPUT_X);
    //int moveY = (int)round(Input::inputs.INPUT_Y);

    if (moveX == -1 && moveX != xPrev && menuSelectionID > 0)
    {
        xPrev = -1;
        menuSelectionID--;
        AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
    }
    else if (moveX == 1 && moveX != xPrev && menuSelectionID < (buttonCount - 1))
    {
        xPrev = 1;
        menuSelectionID++;
        AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
    }
        
    if (moveX == 0)
    {
        xPrev = 0;
    }

    if (pressedSelect)
    {
        resetAnimation();
        switch (menuSelectionID)
        {
            case arcade:
            {
                Global::gameIsArcadeMode = true;
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                //retVal = new CharacterSelect; INCR_NEW("Menu");
                setVisible(false);

                Global::gameMissionNumber = 0;
                Global::gameArcadeIndex = 0;

                Global::levelID = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).first;
                Global::currentCharacterType = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).second;
                Level* currentLevel = &Global::gameLevelData[Global::levelID];
                Global::levelName = currentLevel->fileName;
                Global::levelNameDisplay = currentLevel->displayName;
                Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];
                
                Global::createTitleCard();

                Global::gameArcadePlaytime = 0.0f;

                Global::isNewLevel = true;
                Global::shouldLoadLevel = true;

                retVal = SwitchStack::get();

                break;
            }

            case mission:
                Global::gameIsArcadeMode = false;
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                retVal = new CharacterSelect; INCR_NEW("Menu");
                setVisible(false);
                break;

            case extra:
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                retVal = new Extras; INCR_NEW("Menu");
                setVisible(false);
                break;

            case config:
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                retVal = new ConfigMenu; INCR_NEW("Menu");
                setVisible(false);
                break;

            case exit:
                Global::gameState = STATE_EXITING;
                break;
        }
    }

    draw();

    return retVal;
}
