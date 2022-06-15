#include "missionmenu.hpp"
#include "../engineTester/main.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../loading/loadergl.hpp"
#include "../renderEngine/display.hpp"
#include "../toolbox/input.hpp"
#include "../loading/levelloader.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../audio/audioplayer.hpp"
#include "../audio/source.hpp"
#include "../entities/camera.hpp"
#include "../guis/guiimageresources.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guiimage.hpp"
#include "button.hpp"
#include "mainmenu.hpp"
#include "../toolbox/maths.hpp"

MissionMenu::MissionMenu()
{
    this->fontSize = 0.05f;
    this->loadResources();
    this->visible = true;
    this->offsetCurr = 0.0f;
    this->offsetTarget = 0.0f;
    this->currButtonId = 0;
}

MissionMenu::~MissionMenu()
{
    this->unloadResources();
}

void MissionMenu::loadResources()
{
    if (levelButtons.size() != 0)
    {
        std::fprintf(stdout, "Warning: MissionMenu loading resources when they are already loaded.\n");
        return;
    }

    textureParallelogram              = LoaderGL::loadTexture("res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop      = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramBackdrop.png");
    textureParallelogramHalf          = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalf.png");
    textureParallelogramHalfBackdrop  = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalfBackdrop.png");
    textureParallelogramHalf2         = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalf2.png");
    textureParallelogramHalf2Backdrop = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalf2Backdrop.png");

    float aspectRatio = Display::ASPECT_RATIO;
    levelButtons.clear();

    std::vector<int>* levelIdsToUse = &Global::gameLevelIdsKnuckles;
    switch (Global::currentCharacterType)
    {
        case Global::PlayableCharacter::Sonic: levelIdsToUse = &Global::gameLevelIdsSonic; break;
        case Global::PlayableCharacter::Tails: levelIdsToUse = &Global::gameLevelIdsTails; break;
        default: break;
    }

    for (int i = 0; i < (int)levelIdsToUse->size(); i++)
    {
        Level level = Global::gameLevelData[(*levelIdsToUse)[i]];
        std::string buttonText = "";
        buttonText = level.displayName;
        Button* nextButton = new Button(level.displayName, Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.31f, 0.5f + (0.2f * i), 0.56f / aspectRatio, 0.07f, true); INCR_NEW("Button");
        levelButtons.push_back(nextButton);
    }
    currButtonId = 0;

    textureRankA       = LoaderGL::loadTexture("res/Images/MainMenu/RankA.png");
    textureRankB       = LoaderGL::loadTexture("res/Images/MainMenu/RankB.png");
    textureRankC       = LoaderGL::loadTexture("res/Images/MainMenu/RankC.png");
    textureRankD       = LoaderGL::loadTexture("res/Images/MainMenu/RankD.png");
    textureRankE       = LoaderGL::loadTexture("res/Images/MainMenu/RankE.png");
    textureBlank       = LoaderGL::loadTexture("res/Images/MainMenu/NoRank.png");
    textureSelect      = LoaderGL::loadTexture("res/Images/MainMenu/MissionSelect.png");
    textureNpcFound    = LoaderGL::loadTexture("res/Images/MainMenu/NpcFound.png");
    textureNpcLost     = LoaderGL::loadTexture("res/Images/MainMenu/NpcMissing.png");
    textureEmeraldLost = LoaderGL::loadTexture("res/Images/MainMenu/EmeraldMissing.png");

    const float rankWidth = 0.07f/aspectRatio;
    missionButton = new Button("_____", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.69f, 0.5f, 0.56f / aspectRatio, 0.07f, true); INCR_NEW("Button");
    rankM1        = new GuiImage(textureRankA,  0.69f-rankWidth*1.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiImage");
    rankM2        = new GuiImage(textureRankB,  0.69f-rankWidth*0.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiImage");
    rankM3        = new GuiImage(textureRankC,  0.69f+rankWidth*0.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiImage");
    rankM4        = new GuiImage(textureRankD,  0.69f+rankWidth*1.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiImage");
    missionSelect = new GuiImage(textureSelect, 0.69f-rankWidth*1.5f, 0.4967f, rankWidth*1.5f, rankWidth*aspectRatio*1.5f, 0); INCR_NEW("GuiImage");
    
    npc     = new GuiImage(textureNpcLost,     0.69f-rankWidth*1.0f, 0.4f, 0.15f/aspectRatio, 0.15f, 0); INCR_NEW("GuiImage");
    emerald = new GuiImage(textureEmeraldLost, 0.69f+rankWidth*1.0f, 0.4f, 0.12f/aspectRatio, 0.12f, 0); INCR_NEW("GuiImage");

    timeButton  = new Button("Time: ", Global::fontVipnagorgialla, textureParallelogramHalf2, textureParallelogramHalf2Backdrop, 0.69f + 0.56f/4, 0.65f, 1.12f / aspectRatio, 0.07f, true, true); INCR_NEW("Button");
    scoreButton = new Button("Score:", Global::fontVipnagorgialla, textureParallelogramHalf2, textureParallelogramHalf2Backdrop, 0.69f + 0.56f/4, 0.8f,  1.12f / aspectRatio, 0.07f, true, true); INCR_NEW("Button");
}

void MissionMenu::unloadResources()
{
    if ((int)levelButtons.size() == 0)
    {
        std::fprintf(stdout, "Warning: MissionMenu unloading resources when they are empty.\n");
        return;
    }

    for (int i = 0; i < (int)levelButtons.size(); i++)
    {
        delete levelButtons[i]; levelButtons[i] = nullptr; INCR_DEL("Button");
    }
    levelButtons.clear();
    currButtonId = 0;

    delete missionButton; missionButton = nullptr; INCR_DEL("Button");

    LoaderGL::deleteTexture(textureParallelogram);
    LoaderGL::deleteTexture(textureParallelogramBackdrop);
    LoaderGL::deleteTexture(textureParallelogramHalf);
    LoaderGL::deleteTexture(textureParallelogramHalfBackdrop);
    LoaderGL::deleteTexture(textureParallelogramHalf2);
    LoaderGL::deleteTexture(textureParallelogramHalf2Backdrop);
    textureParallelogram              = GL_NONE;
    textureParallelogramBackdrop      = GL_NONE;
    textureParallelogramHalf          = GL_NONE;
    textureParallelogramHalfBackdrop  = GL_NONE;
    textureParallelogramHalf2         = GL_NONE;
    textureParallelogramHalf2Backdrop = GL_NONE;

    LoaderGL::deleteTexture(textureRankA);
    LoaderGL::deleteTexture(textureRankB);
    LoaderGL::deleteTexture(textureRankC);
    LoaderGL::deleteTexture(textureRankD);
    LoaderGL::deleteTexture(textureRankE);
    LoaderGL::deleteTexture(textureBlank);
    LoaderGL::deleteTexture(textureSelect);
    textureRankA  = GL_NONE;
    textureRankB  = GL_NONE;
    textureRankC  = GL_NONE;
    textureRankD  = GL_NONE;
    textureRankE  = GL_NONE;
    textureBlank  = GL_NONE;
    textureSelect = GL_NONE;

    LoaderGL::deleteTexture(textureNpcFound);
    LoaderGL::deleteTexture(textureNpcLost);
    LoaderGL::deleteTexture(textureEmeraldLost);
    textureNpcFound    = GL_NONE;
    textureNpcLost     = GL_NONE;
    textureEmeraldLost = GL_NONE;

    delete rankM1;        rankM1        = nullptr; INCR_DEL("GuiImage");
    delete rankM2;        rankM2        = nullptr; INCR_DEL("GuiImage");
    delete rankM3;        rankM3        = nullptr; INCR_DEL("GuiImage");
    delete rankM4;        rankM4        = nullptr; INCR_DEL("GuiImage");
    delete missionSelect; missionSelect = nullptr; INCR_DEL("GuiImage");

    delete npc;     npc     = nullptr; INCR_DEL("GuiImage");
    delete emerald; emerald = nullptr; INCR_DEL("GuiImage");

    delete timeButton;  timeButton  = nullptr; INCR_DEL("Button");
    delete scoreButton; scoreButton = nullptr; INCR_DEL("Button");
}

void MissionMenu::draw(bool updateMissionText)
{
    extern float dt;

    this->offsetTarget = -this->currButtonId*0.15f;
    this->offsetCurr = Maths::approach(this->offsetCurr, this->offsetTarget, 15.0f, dt);
    if (fabsf(this->offsetTarget-this->offsetCurr) < 0.002f)
    {
        this->offsetCurr = this->offsetTarget;
    }
    
    if (visible)
    {
        for (int i = 0; i < (int)levelButtons.size(); i++)
        {
            levelButtons[i]->setPos(0.31f, 0.5f + this->offsetCurr + 0.15f*i);
            levelButtons[i]->setVisible(true);
            levelButtons[i]->setHighlight(false);
        }

        levelButtons[currButtonId]->setHighlight(true);

        missionButton->setVisible(true);
        missionButton->setHighlight(false);
        missionButton->setHighlight(true);
        missionButton->getText()->visible = false;

        timeButton->setVisible(true);
        timeButton->setHighlight(false);
        timeButton->setHighlight(true);

        scoreButton->setVisible(true);
        scoreButton->setHighlight(false);
        scoreButton->setHighlight(true);

        std::vector<int>* levelIdsToUse = &Global::gameLevelIdsKnuckles;
        switch (Global::currentCharacterType)
        {
            case Global::PlayableCharacter::Sonic: levelIdsToUse = &Global::gameLevelIdsSonic; break;
            case Global::PlayableCharacter::Tails: levelIdsToUse = &Global::gameLevelIdsTails; break;
            default: break;
        }
        int buttLvlId = (*levelIdsToUse)[currButtonId];
        Level level = Global::gameLevelData[buttLvlId];

        int numMissions = level.numMissions;

        if (numMissions >= 4) { GuiManager::addImageToRender(rankM4); }
        if (numMissions >= 3) { GuiManager::addImageToRender(rankM3); }
        if (numMissions >= 2) { GuiManager::addImageToRender(rankM2); }
        if (numMissions >= 1) { GuiManager::addImageToRender(rankM1); }

        if (numMissions >= 4)
        {
            std::string rank = "";
            rankM4->textureId = textureBlank;
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M4_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M4_RANK"];
                if (rank == "A") { rankM4->textureId = textureRankA; }
                if (rank == "B") { rankM4->textureId = textureRankB; }
                if (rank == "C") { rankM4->textureId = textureRankC; }
                if (rank == "D") { rankM4->textureId = textureRankD; }
                if (rank == "E") { rankM4->textureId = textureRankE; }
            }
        }
        if (numMissions >= 3)
        {
            std::string rank = "";
            rankM3->textureId = textureBlank;
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M3_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M3_RANK"];
                if (rank == "A") { rankM3->textureId = textureRankA; }
                if (rank == "B") { rankM3->textureId = textureRankB; }
                if (rank == "C") { rankM3->textureId = textureRankC; }
                if (rank == "D") { rankM3->textureId = textureRankD; }
                if (rank == "E") { rankM3->textureId = textureRankE; }
            }
        }
        if (numMissions >= 2)
        {
            std::string rank = "";
            rankM2->textureId = textureBlank;
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M2_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M2_RANK"];
                if (rank == "A") { rankM2->textureId = textureRankA; }
                if (rank == "B") { rankM2->textureId = textureRankB; }
                if (rank == "C") { rankM2->textureId = textureRankC; }
                if (rank == "D") { rankM2->textureId = textureRankD; }
                if (rank == "E") { rankM2->textureId = textureRankE; }
            }
        }
        if (numMissions >= 1)
        {
            std::string rank = "";
            rankM1->textureId = textureBlank;
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M1_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M1_RANK"];
                if (rank == "A") { rankM1->textureId = textureRankA; }
                if (rank == "B") { rankM1->textureId = textureRankB; }
                if (rank == "C") { rankM1->textureId = textureRankC; }
                if (rank == "D") { rankM1->textureId = textureRankD; }
                if (rank == "E") { rankM1->textureId = textureRankE; }
            }
        }

        float aspectRatio = Display::ASPECT_RATIO;
        const float rankWidth = 0.07f/aspectRatio;
        missionSelect->setX(0.69f - rankWidth*1.5f + (Global::gameMissionNumber*rankWidth));
        GuiManager::addImageToRender(missionSelect);

        if (updateMissionText)
        {
            std::string missionTimeString  = "ERROR";
            std::string missionScoreString = "ERROR";
            switch (Global::gameMissionNumber)
            {
                case 0: missionTimeString = "_M1_TIME"; missionScoreString = "_M1_SCORE"; break;
                case 1: missionTimeString = "_M2_TIME"; missionScoreString = "_M2_SCORE"; break;
                case 2: missionTimeString = "_M3_TIME"; missionScoreString = "_M3_SCORE"; break;
                case 3: missionTimeString = "_M4_TIME"; missionScoreString = "_M4_SCORE"; break;
                default: break;
            }

            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionTimeString) != Global::gameSaveData.end())
            {
                std::string timeString = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionTimeString];
                float time = std::stof(timeString);

                // Convert float timer to ints of mm:ss:cc
                int toMinutesSeconds = (int)time;
                float toCentiseconds = time * 100.0f;
                int minuteDisplay = toMinutesSeconds / 60;
                int secondDisplay = toMinutesSeconds % 60;
                int centisecondDisplay = (int)toCentiseconds % 100;
                std::string minStr = std::to_string(minuteDisplay);
                std::string secStr = std::to_string(secondDisplay);
                std::string cenStr = std::to_string(centisecondDisplay);

                //pad with 0 if needed
                if (minStr.length() == 1) { minStr = "0" + minStr; }
                if (secStr.length() == 1) { secStr = "0" + secStr; }
                if (cenStr.length() == 1) { cenStr = "0" + cenStr; }

                std::string niceTime = minStr+":"+secStr+"."+cenStr;
                
                timeButton->generateText("Time: "+niceTime);
            }
            else
            {
                timeButton->generateText("Time: ");
            }

            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionScoreString) != Global::gameSaveData.end())
            {
                std::string score = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionScoreString];
                scoreButton->generateText("Score: "+score);
            }
            else
            {
                scoreButton->generateText("Score: ");
            }
        }

        //update npc and emerald
        GuiManager::addImageToRender(emerald);
        if (Global::stageNpcCounts.find(buttLvlId) != Global::stageNpcCounts.end())
        {
            int numNpcs = Global::stageNpcCounts[buttLvlId];
            int foundNpcs = 0;
            for (int i = 0; i < numNpcs; i++)
            {
                std::string key = level.displayName + "_NPC_" + std::to_string(i);
                if (Global::gameSaveData.find(key) != Global::gameSaveData.end())
                {
                    if (Global::gameSaveData[key] == "true")
                    {
                        foundNpcs++;
                    }
                }
            }

            if (foundNpcs == numNpcs)
            {
                npc->textureId = textureNpcFound;
            }
            else
            {
                npc->textureId = textureNpcLost;
            }

            GuiManager::addImageToRender(npc);
        }
    }
}

void MissionMenu::setVisible(bool visibleStatus)
{
    for (int i = 0; i < (int)levelButtons.size(); i++)
    {
        levelButtons[i]->setVisible(visibleStatus);
        levelButtons[i]->setHighlight(visibleStatus);
    }
    missionButton->setVisible(visibleStatus);
    timeButton->setVisible(visibleStatus);
    scoreButton->setVisible(visibleStatus);
    visible = visibleStatus;
}

Menu* MissionMenu::step()
{
    Menu* retVal = nullptr;

    extern float dt;
    static float holdTick;

    bool shouldGoUp = false;
    bool shouldGoDown = false;
    bool shouldGoLeft = false;
    bool shouldGoRight = false;
    bool pressedSelect = false;
    bool pressedBack = false;

    //bool shouldUpdateMissionText = false;

    int moveX = (int)round(Input::inputs.INPUT_X);
    int moveY = (int)round(Input::inputs.INPUT_Y);

    this->setVisible(true);

    if ((Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || 
        (Input::inputs.INPUT_START   && !Input::inputs.INPUT_PREVIOUS_START))
    {
        pressedSelect = true;
    }

    if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2)
    {
        pressedBack = true;
    }

    if (moveX != moveXPrevious)
    {
        if (moveX == -1)
        {
            shouldGoLeft = true;
        }
        else if (moveX == 1)
        {
            shouldGoRight = true;
        }
    }

    if (moveY != moveYPrevious)
    {
        if (moveY == -1)
        {
            shouldGoUp = true;
        }
        else if (moveY == 1)
        {
            shouldGoDown = true;
        }
    }

    moveXPrevious = moveX;
    moveYPrevious = moveY;

    if (moveY == -1)
    {
        this->holdUpTimer += dt;
    }
    else
    {
        this->holdUpTimer = 0.0f;
    }

    if (moveY == 1)
    {
        this->holdDownTimer += dt;
    }
    else
    {
        this->holdDownTimer = 0.0f;
    }

    if (this->holdUpTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.1f)
        {
            shouldGoUp = true;
            holdTick -= 0.1f;
        }
    }

    if (this->holdDownTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.1f)
        {
            shouldGoDown = true;
            holdTick -= 0.1f;
        }
    }


    if (shouldGoUp)
    {
        if (this->currButtonId > 0)
        {
            //shouldUpdateMissionText = true;
            this->currButtonId = this->currButtonId - 1;
            Global::gameMissionNumber = 0;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }
    if (shouldGoDown)
    {
        if (this->currButtonId < ((int)levelButtons.size() - 1))
        {
            //shouldUpdateMissionText = true;
            this->currButtonId = this->currButtonId + 1;
            Global::gameMissionNumber = 0;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }

    if (shouldGoLeft)
    {
        if (Global::gameMissionNumber > 0)
        {
            //shouldUpdateMissionText = true;
            Global::gameMissionNumber--;
            AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
        }
    }
    if (shouldGoRight)
    {
        std::vector<int>* levelIdsToUse = &Global::gameLevelIdsKnuckles;
        switch (Global::currentCharacterType)
        {
            case Global::PlayableCharacter::Sonic: levelIdsToUse = &Global::gameLevelIdsSonic; break;
            case Global::PlayableCharacter::Tails: levelIdsToUse = &Global::gameLevelIdsTails; break;
            default: break;
        }

        int numMissions = Global::gameLevelData[(*levelIdsToUse)[currButtonId]].numMissions;
        if (Global::gameMissionNumber < numMissions-1)
        {
            //shouldUpdateMissionText = true;
            Global::gameMissionNumber++;
            AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
        }
    }

    //have to do true every time in case of getting a new record, it will update when the menu is reloaded
    this->draw(true);//shouldUpdateMissionText);

    if (pressedSelect)
    {
        this->setVisible(false);
        AudioPlayer::play(38, Global::gameCamera->getFadePosition1());

        std::vector<int>* levelIdsToUse = &Global::gameLevelIdsKnuckles;
        switch (Global::currentCharacterType)
        {
            case Global::PlayableCharacter::Sonic: levelIdsToUse = &Global::gameLevelIdsSonic; break;
            case Global::PlayableCharacter::Tails: levelIdsToUse = &Global::gameLevelIdsTails; break;
            default: break;
        }

        Level* currentLevel = &Global::gameLevelData[(*levelIdsToUse)[currButtonId]];
        Global::levelName = currentLevel->fileName;
        Global::levelNameDisplay = currentLevel->displayName;
        Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];
        
        Global::createTitleCard();

        Global::isNewLevel = true;
        Global::shouldLoadLevel = true;
        retVal = SwitchStack::get();
    }
    else if (pressedBack)
    {
        AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
        retVal = PopMenu::get();
    }

    return retVal;
}
