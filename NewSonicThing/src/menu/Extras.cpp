#include "extras.hpp"
#include "mainmenu.hpp"
#include "../engineTester/main.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../toolbox/input.hpp"

Extras::Extras()
{
    selectInputPrevious = false;
    backInputPrevious = false;

    int hours = Global::gameTotalPlaytime/3600;
    int minutes = (Global::gameTotalPlaytime%3600)/60;
    int seconds = Global::gameTotalPlaytime%60;
    std::string sMin = std::to_string(minutes);
    std::string sSec = std::to_string(seconds);
    if (sMin.length() == 1)
    {
        sMin = "0" + sMin;
    }
    if (sSec.length() == 1)
    {
        sSec = "0" + sSec;
    }
    std::string playtime = "Total Playtime: "+std::to_string(hours)+":"+sMin+":"+sSec;

    int totalRingsCompleted = 0;
    totalRingsCompleted += getRingsComp(LVL_TUTORIAL);
    totalRingsCompleted += getRingsComp(LVL_DRY_LAGOON);
    totalRingsCompleted += getRingsComp(LVL_CLOUD_TEMPLE);
    totalRingsCompleted += getRingsComp(LVL_EMERALD_COAST);
    totalRingsCompleted += getRingsComp(LVL_GREEN_FOREST);
    totalRingsCompleted += getRingsComp(LVL_CASTLE_TOWN);
    totalRingsCompleted += getRingsComp(LVL_DRAGON_ROAD);
    totalRingsCompleted += getRingsComp(LVL_METAL_HARBOR);
    totalRingsCompleted += getRingsComp(LVL_PYRAMID_CAVE);
    totalRingsCompleted += getRingsComp(LVL_SWEET_MOUNTAIN);
    totalRingsCompleted += getRingsComp(LVL_GREEN_HILL_ZONE);
    totalRingsCompleted += getRingsComp(LVL_FROG_FOREST);
    totalRingsCompleted += getRingsComp(LVL_CITY_ESCAPE);
    totalRingsCompleted += getRingsComp(LVL_DELFINO_PLAZA);
    totalRingsCompleted += getRingsComp(LVL_SKY_RAIL);
    totalRingsCompleted += getRingsComp(LVL_SEASIDE_HILL);
    totalRingsCompleted += getRingsComp(LVL_RADICAL_HIGHWAY);
    totalRingsCompleted += getRingsComp(LVL_NOKI_BAY);
    totalRingsCompleted += getRingsComp(LVL_SACRED_SKY);
    totalRingsCompleted += getRingsComp(LVL_TWINKLE_CIRCUIT);

    int totalNPCCollected = 0;
    totalNPCCollected += getNPCComp(LVL_TUTORIAL);
    totalNPCCollected += getNPCComp(LVL_DRY_LAGOON);
    totalNPCCollected += getNPCComp(LVL_CLOUD_TEMPLE);
    totalNPCCollected += getNPCComp(LVL_EMERALD_COAST);
    totalNPCCollected += getNPCComp(LVL_GREEN_FOREST);
    totalNPCCollected += getNPCComp(LVL_CASTLE_TOWN);
    totalNPCCollected += getNPCComp(LVL_DRAGON_ROAD);
    totalNPCCollected += getNPCComp(LVL_METAL_HARBOR);
    totalNPCCollected += getNPCComp(LVL_PYRAMID_CAVE);
    totalNPCCollected += getNPCComp(LVL_SWEET_MOUNTAIN);
    totalNPCCollected += getNPCComp(LVL_GREEN_HILL_ZONE);
    totalNPCCollected += getNPCComp(LVL_FROG_FOREST);
    totalNPCCollected += getNPCComp(LVL_CITY_ESCAPE);
    totalNPCCollected += getNPCComp(LVL_DELFINO_PLAZA);
    totalNPCCollected += getNPCComp(LVL_SKY_RAIL);
    totalNPCCollected += getNPCComp(LVL_SEASIDE_HILL);
    totalNPCCollected += getNPCComp(LVL_RADICAL_HIGHWAY);
    totalNPCCollected += getNPCComp(LVL_NOKI_BAY);
    totalNPCCollected += getNPCComp(LVL_SACRED_SKY);
    totalNPCCollected += getNPCComp(LVL_TWINKLE_CIRCUIT);

    int totalEmeraldsCollected = getTotalEmeralds();

    int totalRankACollected = getTotalRankA();

    int ringPerc = (25*totalRingsCompleted)/5858;
    int npcPerc = (25*totalNPCCollected)/48;
    int emeraldPerc = (24*totalEmeraldsCollected)/7;
    int rankPerc = (25*totalRankACollected)/131;
    int arcadeModeFullClear = 1;
    int totalPerc = ringPerc + npcPerc + emeraldPerc + rankPerc + arcadeModeFullClear;

    bool unlockedMighty = (totalPerc == 100);
    float offset = 0.0f;

    if (unlockedMighty)
    {
        offset = -0.05f;

        Global::gameSaveData["UNLOCKED_MIGHTY"] = "true";
    }

    textTime        = new GUIText(playtime,                                                              0.075f, Global::fontVipnagorgialla, 0.5f, 0.15f + offset, 4, true); INCR_NEW("GUIText");
    textRingsAccum  = new GUIText("Total Rings: " + std::to_string(Global::gameAccumulatedRings),        0.075f, Global::fontVipnagorgialla, 0.5f, 0.25f + offset, 4, true); INCR_NEW("GUIText");
    textRingsComp   = new GUIText("Ring Completion: " + std::to_string(totalRingsCompleted) + " / 5858", 0.075f, Global::fontVipnagorgialla, 0.5f, 0.35f + offset, 4, true); INCR_NEW("GUIText");
    textNPC         = new GUIText("NPC: " + std::to_string(totalNPCCollected) + " / 48",                 0.075f, Global::fontVipnagorgialla, 0.5f, 0.45f + offset, 4, true); INCR_NEW("GUIText");
    textEmeralds    = new GUIText("Chaos Emeralds: " + std::to_string(totalEmeraldsCollected) + " / 7",  0.075f, Global::fontVipnagorgialla, 0.5f, 0.55f + offset, 4, true); INCR_NEW("GUIText");
    textRank        = new GUIText("A Ranks: " + std::to_string(totalRankACollected) + " / 131",          0.075f, Global::fontVipnagorgialla, 0.5f, 0.65f + offset, 4, true); INCR_NEW("GUIText");
    textArcadeClear = new GUIText("Arcade Mode True Ending",                                             0.075f, Global::fontVipnagorgialla, 0.5f, 0.75f + offset, 4, true); INCR_NEW("GUIText");
    textCompletion  = new GUIText("Game Completion: " + std::to_string(totalPerc)+ "%",                  0.075f, Global::fontVipnagorgialla, 0.5f, 0.85f + offset, 4, true); INCR_NEW("GUIText");

    if (unlockedMighty)
    {
        if (Global::gameSaveData.find("PLAY_AS") == Global::gameSaveData.end())
        {
            Global::gameSaveData["PLAY_AS"] = "Sonic";
        }

        textCharSelect = new GUIText("Play as: " + Global::gameSaveData["PLAY_AS"], 0.075f, Global::fontVipnagorgialla, 0.5f, 0.9f, 4, true); INCR_NEW("GUIText");
    }

    Global::saveSaveData();
}

Extras::~Extras()
{
    textTime       ->deleteMe(); delete textTime       ; INCR_DEL("GUIText");
    textRingsAccum ->deleteMe(); delete textRingsAccum ; INCR_DEL("GUIText");
    textRingsComp  ->deleteMe(); delete textRingsComp  ; INCR_DEL("GUIText");
    textNPC        ->deleteMe(); delete textNPC        ; INCR_DEL("GUIText");
    textEmeralds   ->deleteMe(); delete textEmeralds   ; INCR_DEL("GUIText");
    textRank       ->deleteMe(); delete textRank       ; INCR_DEL("GUIText");
    textArcadeClear->deleteMe(); delete textArcadeClear; INCR_DEL("GUIText");
    textCompletion ->deleteMe(); delete textCompletion ; INCR_DEL("GUIText");

    if (textCharSelect != nullptr)
    {
        textCharSelect ->deleteMe(); delete textCharSelect ; INCR_DEL("GUIText");
    }
}

Menu* Extras::step()
{
    Menu* retVal = nullptr;

    bool pressedSelect = false;
    bool pressedBack = false;
    bool pressedLeft = false;
    bool pressedRight = false;

    if ((Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || 
        (Input::inputs.INPUT_START   && !Input::inputs.INPUT_PREVIOUS_START))
    {
        pressedSelect = true;
    }

    if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2)
    {
        pressedBack = true;
    }

    if (Input::inputs.INPUT_X < -0.5f && Input::inputs.INPUT_PREVIOUS_X >= -0.5f)
    {
        pressedLeft = true;
    }

    if (Input::inputs.INPUT_X > 0.5f && Input::inputs.INPUT_PREVIOUS_X <= 0.5f)
    {
        pressedRight = true;
    }

    if (pressedRight || pressedLeft)
    {
        if (Global::gameSaveData.find("UNLOCKED_MIGHTY") != Global::gameSaveData.end())
        {
            if (Global::gameSaveData.find("PLAY_AS") != Global::gameSaveData.end())
            {
                if (Global::gameSaveData["PLAY_AS"] == "Sonic")
                {
                    Global::gameSaveData["PLAY_AS"] = "Mighty";
                }
                else
                {
                    Global::gameSaveData["PLAY_AS"] = "Sonic";
                }
            }

            Global::saveSaveData();

            if (textCharSelect != nullptr)
            {
                textCharSelect->deleteMe(); delete textCharSelect; textCharSelect = nullptr; INCR_DEL("GUIText");
            }

            textCharSelect = new GUIText("Play as: " + Global::gameSaveData["PLAY_AS"], 0.075f, Global::fontVipnagorgialla, 0.5f, 0.9f, 4, true); INCR_NEW("GUIText");
        }
    }

    if ((pressedSelect && !selectInputPrevious) ||
        (pressedBack && !backInputPrevious))
    {
        textTime       ->visible = false;
        textRingsAccum ->visible = false;
        textRingsComp  ->visible = false;
        textNPC        ->visible = false;
        textEmeralds   ->visible = false;
        textRank       ->visible = false;
        textArcadeClear->visible = false;
        textCompletion ->visible = false;

        if (textCharSelect != nullptr)
        {
            textCharSelect ->visible = false;
        }
        
        retVal = PopMenu::get();
        Global::menuExtras = nullptr;

        Global::menuMain->setVisible(true);
        Global::menuMain->draw();
    }
    else
    {
        //draw
        textTime       ->visible = true;
        textRingsAccum ->visible = true;
        textRingsComp  ->visible = true;
        textNPC        ->visible = true;
        textEmeralds   ->visible = true;
        textRank       ->visible = true;
        textArcadeClear->visible = true;
        textCompletion ->visible = true;

        if (textCharSelect != nullptr)
        {
            textCharSelect ->visible = true;
        }
    }

    selectInputPrevious = pressedSelect;
    backInputPrevious = pressedBack;

    return retVal;
}

int Extras::getRingsComp(int lvlId)
{
    Level level = Global::gameLevelData[lvlId];

    if (Global::gameSaveData.find(level.displayName+"_RINGS") != Global::gameSaveData.end())
    {
        return std::stoi(Global::gameSaveData[level.displayName+"_RINGS"].c_str());
    }

    return 0;
}

int Extras::getNPCComp(int lvlId)
{
    Level level = Global::gameLevelData[lvlId];

    int foundNpcs = 0;

    if (Global::stageNpcCounts.find(lvlId) != Global::stageNpcCounts.end())
    {
        int numNpcs = Global::stageNpcCounts[lvlId];
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
    }

    return foundNpcs;
}

int Extras::getTotalEmeralds()
{
    int foundEmeralds = 0;

    for (int i = 0; i < 7; i++)
    {
        std::string lookup = "ChaosEmerald_" + std::to_string(i);

        if (Global::gameSaveData.find(lookup) != Global::gameSaveData.end())
        {
            std::string isFound = Global::gameSaveData[lookup];
            if (isFound == "true")
            {
                foundEmeralds++;
            }
        }
    }

    return foundEmeralds;
}

int Extras::getTotalRankA()
{
    int totalRankA = 0;

    for (int i = 0; i < Global::gameLevelIdsSonic.size(); i++)
    {
        Level level = Global::gameLevelData[i];

        if (Global::gameSaveData.find(level.displayName+"_Sonic_M1_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Sonic_M1_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Sonic_M2_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Sonic_M2_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Sonic_M3_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Sonic_M3_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Sonic_M4_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Sonic_M4_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }
    }

    for (int i = 0; i < Global::gameLevelIdsTails.size(); i++)
    {
        Level level = Global::gameLevelData[i];

        if (Global::gameSaveData.find(level.displayName+"_Tails_M1_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Tails_M1_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Tails_M2_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Tails_M2_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Tails_M3_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Tails_M3_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Tails_M4_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Tails_M4_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }
    }

    for (int i = 0; i < Global::gameLevelIdsKnuckles.size(); i++)
    {
        Level level = Global::gameLevelData[i];

        if (Global::gameSaveData.find(level.displayName+"_Knuckles_M1_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Knuckles_M1_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Knuckles_M2_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Knuckles_M2_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Knuckles_M3_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Knuckles_M3_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }

        if (Global::gameSaveData.find(level.displayName+"_Knuckles_M4_RANK") != Global::gameSaveData.end())
        {
            std::string rank = Global::gameSaveData[level.displayName+"_Knuckles_M4_RANK"];
            if (rank == "A")
            {
                totalRankA++;
            }
        }
    }

    return totalRankA;
}
