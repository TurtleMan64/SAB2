#include "hud.h"

#include "../toolbox/input.h"
#include "pausescreen.h"
#include "../engineTester/main.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/guitext.h"
#include "../guis/guimanager.h"
#include "../guis/guitextureresources.h"
#include "../guis/guitexture.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../entities/controllableplayer.h"
#include "../renderEngine/renderEngine.h"
#include "timer.h"

float HUD::bonusTimer = 0.0f;
GuiTexture* HUD::pointBonus = nullptr;
GLuint HUD::pointBonusIds[9];

HUD::HUD()
{
    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;

    this->aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    //this->safeAreaY = 0.015f;
    //this->safeAreaX = safeAreaY;// / this->aspectRatio;

    //this->s = 0.045f; //height of a character
    this->w = s / this->aspectRatio; //width of a single text character

    this->numberLives = new GUINumber(Global::gameLives, safeAreaX + w*2, 1.0f - (safeAreaY + s*0.4f), s, 6, false, 2, false); INCR_NEW("GUINumber");
    this->textLivesMission = new GUIText("Inf", s, Global::fontVipnagorgialla, safeAreaX + w*2, 1.0f - (safeAreaY + s*0.4f), 6, false);  INCR_NEW("GUIText");

    this->numberFPS = new GUINumber(Global::currentCalculatedFPS, 1.0f, 0.0f, s, 2, false, 0, false); INCR_NEW("GUINumber");

    this->numberScore = new GUINumber(Global::gameScore,      safeAreaX,       safeAreaY,       s, 0, false, 8, true);  INCR_NEW("GUINumber");
    this->timer       = new Timer(Global::fontVipnagorgialla, safeAreaX,       safeAreaY + s,   s, 0, false);           INCR_NEW("Timer");
    this->numberRings = new GUINumber(Global::gameRingCount,  safeAreaX + w*2, safeAreaY + s*3, s, 0, false, 3, false); INCR_NEW("GUINumber");
    //printf("creating HUD object\n");
    Global::mainHudTimer = this->timer;

    //this->speedometerScale = 1.5f;

    this->textSpeedUnits = new GUIText("km/h", s, Global::fontVipnagorgialla, 1 - safeAreaX, 1.0f - safeAreaY, 8, false); INCR_NEW("GUIText");
    this->numberSpeed = new GUINumber(Global::gameMainVehicleSpeed, 1.0f - safeAreaX - this->textSpeedUnits->maxLineWidth - 0.003f, 1.0f - safeAreaY, speedometerScale * s, 8, false, 0, false); INCR_NEW("GUINumber");

    GuiTextureResources::textureRing->setX(safeAreaX + w);
    GuiTextureResources::textureRing->setY(safeAreaY + s*3);
    GuiTextureResources::textureRing->setSizeScaled(w*2, s*2);

    GuiTextureResources::textureLifeIcon->setX(safeAreaX + w);
    GuiTextureResources::textureLifeIcon->setY(1.0f - (safeAreaY + s));
    GuiTextureResources::textureLifeIcon->setSizeScaled(w*2, s*2);


    pointBonusIds[0] = Loader::loadTexture("res/Images/PointBonus/0.png");
    pointBonusIds[1] = Loader::loadTexture("res/Images/PointBonus/1.png");
    pointBonusIds[2] = Loader::loadTexture("res/Images/PointBonus/2.png");
    pointBonusIds[3] = Loader::loadTexture("res/Images/PointBonus/3.png");
    pointBonusIds[4] = Loader::loadTexture("res/Images/PointBonus/4.png");
    pointBonusIds[5] = Loader::loadTexture("res/Images/PointBonus/5.png");
    pointBonusIds[6] = Loader::loadTexture("res/Images/PointBonus/6.png");
    pointBonusIds[7] = Loader::loadTexture("res/Images/PointBonus/7.png");
    pointBonusIds[8] = Loader::loadTexture("res/Images/PointBonus/8.png");
    pointBonusIds[9] = Loader::loadTexture("res/Images/PointBonus/9.png");
    pointBonus = new GuiTexture(pointBonusIds[7], safeAreaX + 4*w, safeAreaY + 5*s, 8*w, s, 0); INCR_NEW("GuiTexture")
}

HUD::~HUD()
{
    delete this->timer; INCR_DEL("Timer");
    this->numberFPS->deleteMe(); delete this->numberFPS; INCR_DEL("GUINumber");
    this->numberLives->deleteMe(); delete this->numberLives; INCR_DEL("GUINumber");
    this->numberScore->deleteMe(); delete this->numberScore; INCR_DEL("GUINumber");
    this->numberRings->deleteMe(); delete this->numberRings; INCR_DEL("GUINumber");
    this->numberSpeed->deleteMe(); delete this->numberSpeed; INCR_DEL("GUINumber");
    this->textSpeedUnits->deleteMe(); delete this->textSpeedUnits; INCR_DEL("GUIText");
    this->textLivesMission->deleteMe(); delete this->textLivesMission; INCR_DEL("GUIText");
    delete this->pointBonus; INCR_DEL("GuiTexture");
    for (int i = 0; i < 10; i++)
    {
        Loader::deleteTexture(pointBonusIds[i]);
    }
}

Timer* HUD::getTimer()
{
    return this->timer;
}

void HUD::draw()
{
    //printf("HUd.draw being called\n");
    if (Global::startStageTimer > 0.999f || Global::finishStageTimer >= 9.166f-0.05f)
    {
        this->timer->setVisible(false);
        this->numberFPS->visible = false;
        this->numberLives->visible = false;
        this->numberScore->visible = false;
        this->numberRings->visible = false;
        this->numberSpeed->visible = false;
        this->textSpeedUnits->visible = false;
        this->textLivesMission->visible = false;
    }
    else
    {
        this->timer->setVisible(true);
        this->numberFPS->visible = true;
        this->numberLives->visible = true;
        this->numberScore->visible = true;
        this->numberRings->visible = true;
        this->numberSpeed->visible = true;
        this->textSpeedUnits->visible = true;
        this->textLivesMission->visible = true;

        if (Global::displayFPS)
        {
            this->numberFPS->visible = true;
            this->numberFPS->displayNumber = Global::currentCalculatedFPS;
            this->numberFPS->refresh();
        }
        else
        {
            this->numberFPS->visible = false;
        }
        this->numberLives->displayNumber = Global::gameLives;
        this->numberLives->refresh();
        this->numberRings->displayNumber = Global::gameRingCount;
        this->numberRings->refresh();
        this->numberScore->displayNumber = Global::gameScore;
        this->numberScore->refresh();
        this->numberSpeed->displayNumber = Global::gameMainVehicleSpeed;
        this->numberSpeed->refresh();

        if (Global::gameIsArcadeMode)
        {
            this->textLivesMission->visible = false;
            this->numberLives->visible = true;
        }
        else
        {
            this->textLivesMission->visible = true;
            this->numberLives->visible = false;
        }

        this->timer->refresh();

        GuiManager::addGuiToRender(GuiTextureResources::textureLifeIcon);
        GuiManager::addGuiToRender(GuiTextureResources::textureRing);
    }

    if (bonusTimer > 0.0f)
    {
        float time = 2.0f - bonusTimer;
        float newScale = 1.0f;

        if (time < 0.2f)
        {
            newScale = time*5.0f;
        }
        else if (time < 1.2f)
        {
            newScale = 1.0f;
        }
        else
        {
            newScale = (bonusTimer)*1.25f;
        }

        pointBonus->setScale(newScale*1.5f);

        GuiManager::addGuiToRender(pointBonus);
    }
}

Menu* HUD::step()
{
    //printf("          HUD.step being called\n");
    extern float dt;

    Menu* retVal = nullptr;

    pauseTimer = fmaxf(0.0f, pauseTimer - dt);
    bonusTimer = fmaxf(0.0f, bonusTimer - dt);

    if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START &&
        Global::finishStageTimer < 0.0f &&
        pauseTimer == 0.0f &&
        !Global::gameMainPlayer->isDying())
    {
        pauseTimer = 0.5f;
        retVal = new PauseScreen; INCR_NEW("Menu");
        //printf("          HUD.step going into pause menu\n");
    }

    if (Global::startStageTimer <= 0.0f)
    {
        this->timer->increment();
    }

    this->draw();
    return retVal;
}

void HUD::displayPointBonus(int idx)
{
    bonusTimer = 2.0f;
    if (idx < 0)
    {
        idx = 0;
    }
    else if (idx > 9)
    {
        idx = 9;
    }
    pointBonus->setTexture(pointBonusIds[idx]);
}
