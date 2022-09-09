#include "hud.hpp"

#include "../toolbox/input.hpp"
#include "pausescreen.hpp"
#include "../engineTester/main.hpp"
#include "../fontMeshCreator/guinumber.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guiimageresources.hpp"
#include "../guis/guiimage.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../renderEngine/display.hpp"
#include "../entities/controllableplayer.hpp"
#include "../loading/loadergl.hpp"
#include "timer.hpp"

float HUD::bonusTimer = 0.0f;
GuiImage* HUD::pointBonus = nullptr;
GLuint HUD::pointBonusIds[10] = {0,0,0,0,0,0,0,0,0,0};

float HUD::itemTimer = 0.0f;
GuiImage* HUD::itemImage = nullptr;
GLuint HUD::items[11] = {0,0,0,0,0,0,0,0,0,0,0};

HUD::HUD()
{
    aspectRatio = Display::ASPECT_RATIO;
    //this->safeAreaY = 0.015f;
    //this->safeAreaX = safeAreaY;// / this->aspectRatio;

    //this->s = 0.045f; //height of a character
    this->w = s / aspectRatio; //width of a single text character

    this->numberLives = new GUINumber(Global::gameLives, safeAreaX + w*2, 1.0f - (safeAreaY + s*0.4f), s, 6, false, 2, false); INCR_NEW("GUINumber");
    this->textLivesMission = new GUIText("Inf", s, Global::fontVipnagorgialla, safeAreaX + w*2, 1.0f - (safeAreaY + s*0.4f), 6, false);  INCR_NEW("GUIText");

    this->numberFPS = new GUINumber(Global::currentCalculatedFPS, 1.0f, 0.0f, s, 2, false, 0, false); INCR_NEW("GUINumber");

    this->numberScore = new GUINumber(Global::gameScore,      safeAreaX,       safeAreaY,       s, 0, false, 8, true);  INCR_NEW("GUINumber");
    this->timer       = new Timer(Global::fontVipnagorgialla, safeAreaX,       safeAreaY + s,   s, 0, false);           INCR_NEW("Timer");
    this->numberRings = new GUINumber(Global::gameRingCount,  safeAreaX + w*2, safeAreaY + s*3, s, 0, false, 3, false); INCR_NEW("GUINumber");
    Global::mainHudTimer = this->timer;

    //this->speedometerScale = 1.5f;

    this->textSpeedUnits = new GUIText("km/h", s, Global::fontVipnagorgialla, 1 - safeAreaX, 1.0f - safeAreaY, 8, false); INCR_NEW("GUIText");
    this->numberSpeed = new GUINumber(Global::gameMainVehicleSpeed, 1.0f - safeAreaX - this->textSpeedUnits->maxLineWidth - 0.003f, 1.0f - safeAreaY, speedometerScale * s, 8, false, 0, false); INCR_NEW("GUINumber");

    GuiImageResources::textureRing->setX(safeAreaX + w);
    GuiImageResources::textureRing->setY(safeAreaY + s*3);
    GuiImageResources::textureRing->setSizeScaled(w*2, s*2);

    GuiImageResources::textureLifeIcon->setX(safeAreaX + w);
    GuiImageResources::textureLifeIcon->setY(1.0f - (safeAreaY + s));
    GuiImageResources::textureLifeIcon->setSizeScaled(w*2, s*2);


    pointBonusIds[0] = LoaderGL::loadTexture("res/Images/PointBonus/0.png");
    pointBonusIds[1] = LoaderGL::loadTexture("res/Images/PointBonus/1.png");
    pointBonusIds[2] = LoaderGL::loadTexture("res/Images/PointBonus/2.png");
    pointBonusIds[3] = LoaderGL::loadTexture("res/Images/PointBonus/3.png");
    pointBonusIds[4] = LoaderGL::loadTexture("res/Images/PointBonus/4.png");
    pointBonusIds[5] = LoaderGL::loadTexture("res/Images/PointBonus/5.png");
    pointBonusIds[6] = LoaderGL::loadTexture("res/Images/PointBonus/6.png");
    pointBonusIds[7] = LoaderGL::loadTexture("res/Images/PointBonus/7.png");
    pointBonusIds[8] = LoaderGL::loadTexture("res/Images/PointBonus/8.png");
    pointBonusIds[9] = LoaderGL::loadTexture("res/Images/PointBonus/9.png");
    pointBonus = new GuiImage(pointBonusIds[7], safeAreaX + 4*w, safeAreaY + 5*s, 8*w, s, 0); INCR_NEW("GuiImage");

    items[ 0] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_speed.png");
    items[ 1] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_ring5.png");
    items[ 2] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_1up_sonic.png");
    items[ 3] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_ring10.png");
    items[ 4] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_ring20.png");
    items[ 5] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_nbarria.png");
    items[ 6] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_explosion.png");
    items[ 7] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_health.png");
    items[ 8] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_tbarria.png");
    items[ 9] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/Blank.png");
    items[10] = LoaderGL::loadTexture("res/Models/Objects/ItemBox/Items/item_muteki.png");
    itemImage = new GuiImage(items[0], 0.5f, 0.82f, 0.133f/aspectRatio, 0.133f, 0); INCR_NEW("GuiImage");
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
    delete this->pointBonus; INCR_DEL("GuiImage");
    for (int i = 0; i < 10; i++)
    {
        LoaderGL::deleteTexture(pointBonusIds[i]);
    }
    delete this->itemImage; INCR_DEL("GuiImage");
    for (int i = 0; i < 11; i++)
    {
        LoaderGL::deleteTexture(items[i]);
    }
}

Timer* HUD::getTimer()
{
    return this->timer;
}

void HUD::draw()
{
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

        if (Global::displaySpeedometer)
        {
            this->numberSpeed->visible = true;
            this->textSpeedUnits->visible = true;
            this->numberSpeed->displayNumber = Global::gameMainVehicleSpeed;
            this->numberSpeed->refresh();
        }
        else
        {
            this->numberSpeed->visible = false;
            this->textSpeedUnits->visible = false;
        }

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

        GuiManager::addImageToRender(GuiImageResources::textureLifeIcon);
        GuiManager::addImageToRender(GuiImageResources::textureRing);
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

        GuiManager::addImageToRender(pointBonus);
    }

    if (itemTimer > 0.0f)
    {
        float time = 2.0f - itemTimer;
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
            newScale = (itemTimer)*1.25f;
        }

        itemImage->setScale(newScale);

        GuiManager::addImageToRender(itemImage);
    }
}

Menu* HUD::step()
{
    extern float dt;

    Menu* retVal = nullptr;

    pauseTimer = fmaxf(0.0f, pauseTimer - dt);
    bonusTimer = fmaxf(0.0f, bonusTimer - dt);
    itemTimer  = fmaxf(0.0f, itemTimer  - dt);

    if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START &&
        Global::finishStageTimer < 0.0f &&
        pauseTimer == 0.0f &&
        !Global::gameMainPlayer->isDying())
    {
        pauseTimer = 0.5f;
        retVal = new PauseScreen; INCR_NEW("Menu");
    }

    if (Global::startStageTimer <= 0.0f)
    {
        timer->increment();
    }

    draw();
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
    pointBonus->textureId = pointBonusIds[idx];
}

void HUD::displayItem(int idx)
{
    itemTimer = 2.0f;
    if (idx < 0)
    {
        idx = 0;
    }
    else if (idx > 10)
    {
        idx = 10;
    }
    itemImage->textureId = items[idx];
}

