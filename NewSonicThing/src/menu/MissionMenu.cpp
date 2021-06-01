#include "missionmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <iostream>
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

MissionMenu::MissionMenu()
{
    //std::cout << "Initializing Mission Menu\n";
    this->fontSize = 0.05f;
    this->loadResources();
    this->visible = true;
    this->offsetCurr = 0.0f;
    this->offsetTarget = 0.0f;
    this->currButtonID = 0;
    //std::cout << "Mission Menu initialized\n";
}

MissionMenu::~MissionMenu()
{
    //std::cout << "Deleting Mission Menu\n";
    this->unloadResources();
    //std::cout << "Mission Menu deleted.\n";
}

void MissionMenu::loadResources()
{
    if (levelButtons.size() != 0)
    {
        std::fprintf(stdout, "Warning: MissionMenu loading resources when they are already loaded.\n");
        return;
    }

    textureParallelogram              = Loader::loadTexture("res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop      = Loader::loadTexture("res/Images/MainMenu/ParallelogramBackdrop.png");
    textureParallelogramHalf          = Loader::loadTexture("res/Images/MainMenu/ParallelogramHalf.png");
    textureParallelogramHalfBackdrop  = Loader::loadTexture("res/Images/MainMenu/ParallelogramHalfBackdrop.png");
    textureParallelogramHalf2         = Loader::loadTexture("res/Images/MainMenu/ParallelogramHalf2.png");
    textureParallelogramHalf2Backdrop = Loader::loadTexture("res/Images/MainMenu/ParallelogramHalf2Backdrop.png");

    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;
    float aspectRatio = (float)SCR_WIDTH / SCR_HEIGHT;
    levelButtons.clear();

    std::vector<int>* levelIDsToUse = &Global::gameLevelIdsKnuckles;
    switch (Global::currentCharacterType)
    {
        case Global::PlayableCharacter::Sonic: levelIDsToUse = &Global::gameLevelIdsSonic; break;
        case Global::PlayableCharacter::Tails: levelIDsToUse = &Global::gameLevelIdsTails; break;
        default: break;
    }

    for (int i = 0; i < (int)levelIDsToUse->size(); i++)
    {
        Level level = Global::gameLevelData[(*levelIDsToUse)[i]];
        std::string buttonText = "";
        buttonText = level.displayName;
        Button* nextButton = new Button(level.displayName, Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.31f, 0.5f + (0.2f * i), 0.56f / aspectRatio, 0.07f, true); INCR_NEW("Button");
        levelButtons.push_back(nextButton);
    }
    currButtonID = 0;

    textureRankA  = Loader::loadTexture("res/Images/MainMenu/RankA.png");
    textureRankB  = Loader::loadTexture("res/Images/MainMenu/RankB.png");
    textureRankC  = Loader::loadTexture("res/Images/MainMenu/RankC.png");
    textureRankD  = Loader::loadTexture("res/Images/MainMenu/RankD.png");
    textureRankE  = Loader::loadTexture("res/Images/MainMenu/RankE.png");
    textureBlank  = Loader::loadTexture("res/Images/MainMenu/NoRank.png");
    textureSelect = Loader::loadTexture("res/Images/MainMenu/MissionSelect.png");
    textureNpcFound = Loader::loadTexture("res/Images/MainMenu/NpcFound.png");
    textureNpcLost = Loader::loadTexture("res/Images/MainMenu/NpcMissing.png");
    textureEmeraldLost = Loader::loadTexture("res/Images/MainMenu/EmeraldMissing.png");

    const float rankWidth = 0.07f/aspectRatio;
    missionButton = new Button("_____", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.69f, 0.5f, 0.56f / aspectRatio, 0.07f, true); INCR_NEW("Button");
    rankM1        = new GuiTexture(textureRankA,  0.69f-rankWidth*1.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiTexture");
    rankM2        = new GuiTexture(textureRankB,  0.69f-rankWidth*0.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiTexture");
    rankM3        = new GuiTexture(textureRankC,  0.69f+rankWidth*0.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiTexture");
    rankM4        = new GuiTexture(textureRankD,  0.69f+rankWidth*1.5f, 0.4967f, rankWidth,      rankWidth*aspectRatio,      0); INCR_NEW("GuiTexture");
    missionSelect = new GuiTexture(textureSelect, 0.69f-rankWidth*1.5f, 0.4967f, rankWidth*1.5f, rankWidth*aspectRatio*1.5f, 0); INCR_NEW("GuiTexture");
    
    npc     = new GuiTexture(textureNpcLost,     0.69f-rankWidth*1.0f, 0.4f, 0.15f/aspectRatio, 0.15f, 0); INCR_NEW("GuiTexture");
    emerald = new GuiTexture(textureEmeraldLost, 0.69f+rankWidth*1.0f, 0.4f, 0.12f/aspectRatio, 0.12f, 0); INCR_NEW("GuiTexture");

    timeButton  = new Button("Time: ", Global::fontVipnagorgialla, textureParallelogramHalf2, textureParallelogramHalf2Backdrop, 0.69f + 0.56f/4, 0.65f, 1.12f / aspectRatio, 0.07f, true, true); INCR_NEW("Button");
    scoreButton = new Button("Score:", Global::fontVipnagorgialla, textureParallelogramHalf2, textureParallelogramHalf2Backdrop, 0.69f + 0.56f/4, 0.8f,  1.12f / aspectRatio, 0.07f, true, true); INCR_NEW("Button");
}

void MissionMenu::unloadResources()
{
    //std::cout << "Unloading Mission Menu resources.\n";
    if ((int)levelButtons.size() == 0)
    {
        std::fprintf(stdout, "Warning: MissionMenu unloading resources when they are empty.\n");
        return;
    }

    GuiManager::clearGuisToRender();

    for (int i = 0; i < (int)levelButtons.size(); i++)
    {
        delete levelButtons[i]; levelButtons[i] = nullptr; INCR_DEL("Button");
    }
    levelButtons.clear();
    currButtonID = 0;

    delete missionButton; missionButton = nullptr; INCR_DEL("Button");

    Loader::deleteTexture(textureParallelogram);
    Loader::deleteTexture(textureParallelogramBackdrop);
    Loader::deleteTexture(textureParallelogramHalf);
    Loader::deleteTexture(textureParallelogramHalfBackdrop);
    Loader::deleteTexture(textureParallelogramHalf2);
    Loader::deleteTexture(textureParallelogramHalf2Backdrop);
    textureParallelogram              = GL_NONE;
    textureParallelogramBackdrop      = GL_NONE;
    textureParallelogramHalf          = GL_NONE;
    textureParallelogramHalfBackdrop  = GL_NONE;
    textureParallelogramHalf2         = GL_NONE;
    textureParallelogramHalf2Backdrop = GL_NONE;

    Loader::deleteTexture(textureRankA);
    Loader::deleteTexture(textureRankB);
    Loader::deleteTexture(textureRankC);
    Loader::deleteTexture(textureRankD);
    Loader::deleteTexture(textureRankE);
    Loader::deleteTexture(textureBlank);
    Loader::deleteTexture(textureSelect);
    textureRankA  = GL_NONE;
    textureRankB  = GL_NONE;
    textureRankC  = GL_NONE;
    textureRankD  = GL_NONE;
    textureRankE  = GL_NONE;
    textureBlank  = GL_NONE;
    textureSelect = GL_NONE;

    Loader::deleteTexture(textureNpcFound);
    Loader::deleteTexture(textureNpcLost);
    Loader::deleteTexture(textureEmeraldLost);
    textureNpcFound = GL_NONE;
    textureNpcLost = GL_NONE;
    textureEmeraldLost = GL_NONE;

    delete rankM1;        rankM1        = nullptr; INCR_DEL("GuiTexture");
    delete rankM2;        rankM2        = nullptr; INCR_DEL("GuiTexture");
    delete rankM3;        rankM3        = nullptr; INCR_DEL("GuiTexture");
    delete rankM4;        rankM4        = nullptr; INCR_DEL("GuiTexture");
    delete missionSelect; missionSelect = nullptr; INCR_DEL("GuiTexture");

    delete npc;     npc     = nullptr; INCR_DEL("GuiTexture");
    delete emerald; emerald = nullptr; INCR_DEL("GuiTexture");

    delete timeButton;  timeButton  = nullptr; INCR_DEL("Button");
    delete scoreButton; scoreButton = nullptr; INCR_DEL("Button");

    //std::cout << "Mission Menu resources deleted.\n";
}

void MissionMenu::draw(bool updateMissionText)
{
    extern float dt;

    this->offsetTarget = -this->currButtonID*0.15f;
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

        levelButtons[currButtonID]->setHighlight(true);

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

        std::vector<int>* levelIDsToUse = &Global::gameLevelIdsKnuckles;
        switch (Global::currentCharacterType)
        {
            case Global::PlayableCharacter::Sonic: levelIDsToUse = &Global::gameLevelIdsSonic; break;
            case Global::PlayableCharacter::Tails: levelIDsToUse = &Global::gameLevelIdsTails; break;
            default: break;
        }
        int buttLvlID = (*levelIDsToUse)[currButtonID];
        Level level = Global::gameLevelData[buttLvlID];

        int numMissions = level.numMissions;
        switch (numMissions)
        {
            case 4: GuiManager::addGuiToRender(rankM4);
            case 3: GuiManager::addGuiToRender(rankM3);
            case 2: GuiManager::addGuiToRender(rankM2);
            case 1: GuiManager::addGuiToRender(rankM1);
            default: break;
        }

        if (numMissions >= 4)
        {
            std::string rank = "";
            rankM4->setTexture(textureBlank);
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M4_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M4_RANK"];
                if (rank == "A") rankM4->setTexture(textureRankA);
                if (rank == "B") rankM4->setTexture(textureRankB);
                if (rank == "C") rankM4->setTexture(textureRankC);
                if (rank == "D") rankM4->setTexture(textureRankD);
                if (rank == "E") rankM4->setTexture(textureRankE);
            }
        }
        if (numMissions >= 3)
        {
            std::string rank = "";
            rankM3->setTexture(textureBlank);
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M3_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M3_RANK"];
                if (rank == "A") rankM3->setTexture(textureRankA);
                if (rank == "B") rankM3->setTexture(textureRankB);
                if (rank == "C") rankM3->setTexture(textureRankC);
                if (rank == "D") rankM3->setTexture(textureRankD);
                if (rank == "E") rankM3->setTexture(textureRankE);
            }
        }
        if (numMissions >= 2)
        {
            std::string rank = "";
            rankM2->setTexture(textureBlank);
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M2_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M2_RANK"];
                if (rank == "A") rankM2->setTexture(textureRankA);
                if (rank == "B") rankM2->setTexture(textureRankB);
                if (rank == "C") rankM2->setTexture(textureRankC);
                if (rank == "D") rankM2->setTexture(textureRankD);
                if (rank == "E") rankM2->setTexture(textureRankE);
            }
        }
        if (numMissions >= 1)
        {
            std::string rank = "";
            rankM1->setTexture(textureBlank);
            if (Global::gameSaveData.find(level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M1_RANK") != Global::gameSaveData.end())
            {
                rank = Global::gameSaveData[level.displayName+"_"+Global::characterNames[Global::currentCharacterType]+"_M1_RANK"];
                if (rank == "A") rankM1->setTexture(textureRankA);
                if (rank == "B") rankM1->setTexture(textureRankB);
                if (rank == "C") rankM1->setTexture(textureRankC);
                if (rank == "D") rankM1->setTexture(textureRankD);
                if (rank == "E") rankM1->setTexture(textureRankE);
            }
        }

        extern unsigned int SCR_WIDTH;
        extern unsigned int SCR_HEIGHT;
        float aspectRatio = (float)SCR_WIDTH / SCR_HEIGHT;
        const float rankWidth = 0.07f/aspectRatio;
        missionSelect->setX(0.69f - rankWidth*1.5f + (Global::gameMissionNumber*rankWidth));
        GuiManager::addGuiToRender(missionSelect);

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
        GuiManager::addGuiToRender(emerald);
        if (Global::stageNpcCounts.find(buttLvlID) != Global::stageNpcCounts.end())
        {
            int numNpcs = Global::stageNpcCounts[buttLvlID];
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
                npc->setTexture(textureNpcFound);
            }
            else
            {
                npc->setTexture(textureNpcLost);
            }

            GuiManager::addGuiToRender(npc);
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
        if (this->currButtonID > 0)
        {
            //shouldUpdateMissionText = true;
            this->currButtonID = this->currButtonID - 1;
            Global::gameMissionNumber = 0;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }
    if (shouldGoDown)
    {
        if (this->currButtonID < ((int)levelButtons.size() - 1))
        {
            //shouldUpdateMissionText = true;
            this->currButtonID = this->currButtonID + 1;
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
        std::vector<int>* levelIDsToUse = &Global::gameLevelIdsKnuckles;
        switch (Global::currentCharacterType)
        {
            case Global::PlayableCharacter::Sonic: levelIDsToUse = &Global::gameLevelIdsSonic; break;
            case Global::PlayableCharacter::Tails: levelIDsToUse = &Global::gameLevelIdsTails; break;
            default: break;
        }

        int numMissions = Global::gameLevelData[(*levelIDsToUse)[currButtonID]].numMissions;
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

        std::vector<int>* levelIDsToUse = &Global::gameLevelIdsKnuckles;
        switch (Global::currentCharacterType)
        {
            case Global::PlayableCharacter::Sonic: levelIDsToUse = &Global::gameLevelIdsSonic; break;
            case Global::PlayableCharacter::Tails: levelIDsToUse = &Global::gameLevelIdsTails; break;
            default: break;
        }

        Level* currentLevel = &Global::gameLevelData[(*levelIDsToUse)[currButtonID]];
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
