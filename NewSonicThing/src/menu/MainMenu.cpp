#include "mainmenu.hpp"
#include "../engineTester/main.hpp"

#include <cmath>
#include <iostream>

#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../loading/loadergl.hpp"
#include "../renderEngine/display.hpp"
#include "../toolbox/input.hpp"
#include "../loading/levelloader.hpp"
#include "button.hpp"
#include "../audio/audioplayer.hpp"
#include "../audio/source.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particletexture.hpp"
#include "../entities/camera.hpp"
#include "../particles/particlemaster.hpp"
#include "../guis/guiimageresources.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guiimage.hpp"
#include "missionmenu.hpp"
#include "configmenu.hpp"
#include "extras.hpp"
#include "characterselect.hpp"
#include "resultsscreen.hpp"

MainMenu::MainMenu()
{
    aspectRatio = Display::ASPECT_RATIO;
    buttonCount = 0;
    menuSelectionId = 0;
    textureParallelogram = GL_NONE;
    textureParallelogramBackdrop = GL_NONE;
    textureLogo = GL_NONE;
    loadResources();
    setVisible(false);

    if (Global::menuMain != nullptr)
    {
        printf("Warning: Main Menu should be null but is not.\n");
    }

    Global::menuMain = this;
}

MainMenu::~MainMenu()
{
    unloadResources();

    Global::menuMain = nullptr;
}

void MainMenu::loadResources()
{
    if (menuButtons.size() != 0)
    {
        printf("Main Menu trying to load resources when they are already loaded!\n");
        return;
    }

    textureParallelogram         = LoaderGL::loadTexture("res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramBackdrop.png");
    textureLogo                  = LoaderGL::loadTexture("res/Images/MainMenu/Logo2.png");
    logo = new GuiImage(textureLogo, 0.75f, 0.3f, 0.45f * (3000.0f / 1500.0f) / aspectRatio, 0.45f, 0.0f); INCR_NEW("GuiImage");

    float fontScale = 0.08f;

    // Adding a string here *should* add a button.
    std::vector<std::string> buttonNames
    (
        {
            "ARCADE",
            "MISSION",
            "EXTRA",
            "SETTINGS",
            "EXIT"
        }
    );

    int numButtonsToMake = (int) buttonNames.size();

    menuButtons.clear();

    for (int i = 0; i < numButtonsToMake; i++)
    {
        menuButtons.push_back(new Button(buttonNames[i], Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop,
            0.5f + (0.5f * (i - menuSelectionId) / aspectRatio), 0.8f, (fontScale * 8.0f / aspectRatio), fontScale, true)); INCR_NEW("Button");
    }

    buttonCount = numButtonsToMake;
}

void MainMenu::draw()
{
    extern float dt;

    if (oldSelection != menuSelectionId)
    {
        animationTime = 0.25f;
        animating = true;
        if (oldSelection < menuSelectionId)
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

    if (visible)
    {
        GuiManager::addImageToRender(logo);

        float animationOffset = animationDirection * 12 * (animationTime) * (animationTime);

        for (int i = 0; i < buttonCount; i++)
        {
            menuButtons[i]->setPos(0.5f + (0.75f * (i - menuSelectionId) / aspectRatio) + animationOffset / aspectRatio, 0.8f);
            menuButtons[i]->setVisible(true);
            menuButtons[i]->setHighlight(false);
        }

        menuButtons[menuSelectionId]->setHighlight(true);
    }

    oldSelection = menuSelectionId;
}

void MainMenu::setVisible(bool visibleStatus)
{
    for (int i = 0; i < buttonCount; i++)
    {
        menuButtons[i]->setVisible(visibleStatus);
    }
    visible = visibleStatus;
}

void MainMenu::unloadResources()
{
    if (menuButtons.size() == 0)
    {
        printf("Main Menu trying to unload resources when they are already unloaded!\n");
        return;
    }

    int numToDelete = buttonCount;

    for (int i = 0; i < numToDelete; i++)
    {
        delete menuButtons[i]; INCR_DEL("Button");
    }

    menuButtons.clear();

    LoaderGL::deleteTexture(textureParallelogram);
    LoaderGL::deleteTexture(textureParallelogramBackdrop);
    LoaderGL::deleteTexture(textureLogo);
    textureParallelogram         = GL_NONE;
    textureParallelogramBackdrop = GL_NONE;
    textureLogo                  = GL_NONE;
    logo->textureId              = GL_NONE;
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

    if (moveX == -1 && moveX != xPrev && menuSelectionId > 0)
    {
        xPrev = -1;
        menuSelectionId--;
        AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
    }
    else if (moveX == 1 && moveX != xPrev && menuSelectionId < (buttonCount - 1))
    {
        xPrev = 1;
        menuSelectionId++;
        AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
    }
        
    if (moveX == 0)
    {
        xPrev = 0;
    }

    if (pressedSelect)
    {
        resetAnimation();
        switch (menuSelectionId)
        {
            case arcade:
            {
                Global::gameIsArcadeMode = true;
                MenuManager::arcadeModeIsDoneBadEnding = false;
                MenuManager::arcadeModeIsDoneGoodEnding = false;
                MenuManager::playerFailedArcadeMode = false;
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                setVisible(false);

                Global::gameMissionNumber = 0;
                Global::gameArcadeIndex = 0;

                for (int i = 0; i < 7; i++)
                {
                    Global::arcadeModeEmeraldFound[i] = false;
                }

                Global::levelId = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).first;
                Global::currentCharacterType = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).second;
                Level* currentLevel = &Global::gameLevelData[Global::levelId];
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
            {
                Global::gameIsArcadeMode = false;
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                Global::menuCharacterSelect = new CharacterSelect; INCR_NEW("Menu");
                retVal = Global::menuCharacterSelect;
                setVisible(false);
                Global::menuCharacterSelect->draw();
                animationTime = 0.00f;
                animating = false;
                break;
            }

            case extra:
            {
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                Global::menuExtras = new Extras; INCR_NEW("Menu");
                retVal = Global::menuExtras;

                //Global::menuResults = new ResultsScreen; INCR_NEW("Menu");
                //retVal = Global::menuResults;

                setVisible(false);
                Global::menuExtras->draw();
                animationTime = 0.00f;
                animating = false;
                break;
            }

            case config:
            {
                AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
                Global::menuConfig = new ConfigMenu; INCR_NEW("Menu");
                retVal = Global::menuConfig;
                setVisible(false);
                Global::menuConfig->draw();
                animationTime = 0.00f;
                animating = false;
                break;
            }

            case exit:
            {
                Global::gameState = STATE_EXITING;
                break;
            }

            default: break;
        }
    }

    draw();

    return retVal;
}
