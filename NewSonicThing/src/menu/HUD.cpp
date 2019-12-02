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
#include "timer.h"

HUD::HUD()
{
    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;

    this->aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    this->safeAreaY = 0.015f;
    this->safeAreaX = safeAreaY;// / this->aspectRatio;

    this->s = 0.045f; //height of a character
    this->w = s / this->aspectRatio; //width of a single text character

    this->numberLives = new GUINumber(Global::gameLives, safeAreaX + w*2, 1.0f - (safeAreaY + s*0.4f), s, 6, false, 2, false); INCR_NEW("GUINumber");
    this->textLivesMission = new GUIText("Inf", s, Global::fontVipnagorgialla, safeAreaX + w*2, 1.0f - (safeAreaY + s*0.4f), 6, false);  INCR_NEW("GUIText");

    this->numberFPS = new GUINumber(Global::currentCalculatedFPS, 1.0f, 0.0f, s, 2, false, 0, false); INCR_NEW("GUINumber");

    this->numberScore = new GUINumber(Global::gameScore,      safeAreaX,       safeAreaY,       s, 0, false, 8, true);  INCR_NEW("GUINumber");
    this->timer       = new Timer(Global::fontVipnagorgialla, safeAreaX,       safeAreaY + s,   s, 0, false);           INCR_NEW("Timer");
    this->numberRings = new GUINumber(Global::gameRingCount,  safeAreaX + w*2, safeAreaY + s*3, s, 0, false, 3, false); INCR_NEW("GUINumber");
    Global::mainHudTimer = this->timer;

    this->speedometerScale = 1.5f;

    this->textSpeedUnits = new GUIText("km/h", s, Global::fontVipnagorgialla, 1 - safeAreaX, 1.0f - safeAreaY, 8, false); INCR_NEW("GUIText");
    this->numberSpeed = new GUINumber(Global::gameMainVehicleSpeed, 1.0f - safeAreaX - this->textSpeedUnits->maxLineWidth - 0.003f, 1.0f - safeAreaY, speedometerScale * s, 8, false, 0, false); INCR_NEW("GUINumber");

    GuiTextureResources::textureRing->setX(safeAreaX + w);
    GuiTextureResources::textureRing->setY(safeAreaY + s*3);
    GuiTextureResources::textureRing->setSizeScaled(w*2, s*2);

    GuiTextureResources::textureLifeIcon->setX(safeAreaX + w);
    GuiTextureResources::textureLifeIcon->setY(1.0f - (safeAreaY + s));
    GuiTextureResources::textureLifeIcon->setSizeScaled(w*2, s*2);
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
}

Timer* HUD::getTimer()
{
    return this->timer;
}

void HUD::draw()
{
    if (Global::startStageTimer > 0.999f)
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

        GuiManager::clearGuisToRender();
        GuiManager::addGuiToRender(GuiTextureResources::textureLifeIcon);
        GuiManager::addGuiToRender(GuiTextureResources::textureRing);
    }
}

Menu* HUD::step()
{
    Menu* retVal = nullptr;

    if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START &&
        Global::finishStageTimer < 0.0f)
    {
        retVal = new PauseScreen(this); INCR_NEW("Menu");
    }

    if (Global::startStageTimer <= 0.0f)
    {
        this->timer->increment();
    }

    this->draw();
    return retVal;
}
