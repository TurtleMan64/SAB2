#include "emeraldmanager.h"
#include "entity.h"
#include "emeraldpiece.h"
#include "../engineTester/main.h"
#include "../guis/guimanager.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/vector.h"
#include "../entities/camera.h"
#include "../guis/guirenderer.h"
#include "../guis/guitextureresources.h"
#include "../guis/guitexture.h"
#include "../menu/timer.h"
#include "../renderEngine/loader.h"
#include "../renderEngine/display.h"

#include <vector>
#include <list>
#include <unordered_map>

EmeraldPiece* EmeraldManager::piece1 = nullptr;
EmeraldPiece* EmeraldManager::piece2 = nullptr;
EmeraldPiece* EmeraldManager::piece3 = nullptr;
int   EmeraldManager::piecesRemaining = 3;
float EmeraldManager::pingTimer1 = 0;
float EmeraldManager::pingTimer2 = 0;
float EmeraldManager::pingTimer3 = 0;

GLuint EmeraldManager::radarGreyId   = 0;
GLuint EmeraldManager::radarBlueId   = 0;
GLuint EmeraldManager::radarGreenId  = 0;
GLuint EmeraldManager::radarYellowId = 0;
GLuint EmeraldManager::radarRedId    = 0;
GLuint EmeraldManager::nearPieceId   = 0;

GuiTexture* EmeraldManager::radar1    = nullptr;
GuiTexture* EmeraldManager::radar2    = nullptr;
GuiTexture* EmeraldManager::radar3    = nullptr;
GuiTexture* EmeraldManager::nearPiece = nullptr;

EmeraldManager::~EmeraldManager()
{
    if (EmeraldManager::radar1 != nullptr)
    {
        GuiManager::removeGui(EmeraldManager::radar1);
        delete EmeraldManager::radar1; INCR_DEL("GuiTexture");
        EmeraldManager::radar1 = nullptr;
    }
    if (EmeraldManager::radar2 != nullptr)
    {
        GuiManager::removeGui(EmeraldManager::radar2);
        delete EmeraldManager::radar2; INCR_DEL("GuiTexture");
        EmeraldManager::radar2 = nullptr;
    }
    if (EmeraldManager::radar3 != nullptr)
    {
        GuiManager::removeGui(EmeraldManager::radar3);
        delete EmeraldManager::radar3; INCR_DEL("GuiTexture");
        EmeraldManager::radar3 = nullptr;
    }
    if (EmeraldManager::nearPiece != nullptr)
    {
        GuiManager::removeGui(EmeraldManager::nearPiece);
        delete EmeraldManager::nearPiece; INCR_DEL("GuiTexture");
        EmeraldManager::nearPiece = nullptr;
    }

    Loader::deleteTexture(EmeraldManager::radarGreyId);
    Loader::deleteTexture(EmeraldManager::radarBlueId);
    Loader::deleteTexture(EmeraldManager::radarGreenId);
    Loader::deleteTexture(EmeraldManager::radarYellowId);
    Loader::deleteTexture(EmeraldManager::radarRedId);
    Loader::deleteTexture(EmeraldManager::nearPieceId);
    
    EmeraldManager::radarGreyId   = 0;
    EmeraldManager::radarBlueId   = 0;
    EmeraldManager::radarGreenId  = 0;
    EmeraldManager::radarYellowId = 0;
    EmeraldManager::radarRedId    = 0;
    EmeraldManager::nearPieceId   = 0;
}

EmeraldManager::EmeraldManager()
{
    //Load images of radar
    EmeraldManager::radarGreyId   = Loader::loadTexture("res/Images/TreasureHunting/RadarGrey.png");
    EmeraldManager::radarBlueId   = Loader::loadTexture("res/Images/TreasureHunting/RadarBlue.png");
    EmeraldManager::radarGreenId  = Loader::loadTexture("res/Images/TreasureHunting/RadarGreen.png");
    EmeraldManager::radarYellowId = Loader::loadTexture("res/Images/TreasureHunting/RadarYellow.png");
    EmeraldManager::radarRedId    = Loader::loadTexture("res/Images/TreasureHunting/RadarRed.png");
    EmeraldManager::nearPieceId   = Loader::loadTexture("res/Images/TreasureHunting/NearPiece.png");

    float r = Display::ASPECT_RATIO; //screen ratio
    float s = 0.05f; //size of radar tiles

    //Create Radar gui textures
    EmeraldManager::radar1 = new GuiTexture(EmeraldManager::radarGreyId, 0.5f-((1.5f*s)/r), 1-s, s/r, s, 0); INCR_NEW("GuiTexture");
    EmeraldManager::radar2 = new GuiTexture(EmeraldManager::radarGreyId, 0.5f,              1-s, s/r, s, 0); INCR_NEW("GuiTexture");
    EmeraldManager::radar3 = new GuiTexture(EmeraldManager::radarGreyId, 0.5f+((1.5f*s)/r), 1-s, s/r, s, 0); INCR_NEW("GuiTexture");

    EmeraldManager::nearPiece = new GuiTexture(EmeraldManager::nearPieceId, 0, 0, 0.1f/r, 0.1f, 0); INCR_NEW("GuiTexture");

    std::vector<EmeraldPiece*> piece1List;
    std::vector<EmeraldPiece*> piece2List;
    std::vector<EmeraldPiece*> piece3List;

    int totalPieces = 0;
    EmeraldManager::pingTimer1 = 0.0f;
    EmeraldManager::pingTimer2 = 0.0f;
    EmeraldManager::pingTimer3 = 0.0f;
    EmeraldManager::piece1 = nullptr;
    EmeraldManager::piece2 = nullptr;
    EmeraldManager::piece3 = nullptr;

    visible = false;

    extern std::list<Entity*> gameEntitiesToAdd;

    for (Entity* e : gameEntitiesToAdd)
    {
        if (e->isEmeraldPiece()) 
        {
            EmeraldPiece* piece = (EmeraldPiece*)e;
            if (Global::gameIsNormalMode)
            {
                if (piece->isHardModePiece())
                {
                    Global::deleteEntity(piece);
                }
                else
                {
                    totalPieces++;
                    switch (piece->getPieceNumber())
                    {
                        case 1:  piece1List.push_back(piece); break;
                        case 2:  piece2List.push_back(piece); break;
                        default: piece3List.push_back(piece); break;
                    }
                }
            }
            else if (Global::gameIsHardMode)
            {
                if (piece->isHardModePiece())
                {
                    totalPieces++;
                }
                else
                {
                    Global::deleteEntity(piece);
                }
            }
        }
    }

    if (Global::gameIsHardMode)
    {
        EmeraldManager::piecesRemaining = totalPieces;

        EmeraldManager::radar1->setVisible(false);
        EmeraldManager::radar2->setVisible(false);
        EmeraldManager::radar3->setVisible(false);
    }
    else if (Global::gameIsNormalMode)
    {
        //Pick a random p1
        EmeraldManager::piece1 = piece1List[(int)(Maths::random()*piece1List.size())];
        Vector3f p1Loc(EmeraldManager::piece1->getPosition());

        //Calculate distances from p1 to p2
        std::vector<float> distTo2;
        for (EmeraldPiece* piece : piece2List)
        {
            Vector3f p2Loc(piece->getPosition());
            Vector3f diff = p1Loc - p2Loc;
            float dist = diff.lengthSquared();

            distTo2.push_back(dist);
        }

        //Get the minimum distance needed to be a valid p2
        std::sort(distTo2.begin(), distTo2.end());
        float minDist = distTo2[distTo2.size()/2];

        //Generate valid p2's
        std::vector<EmeraldPiece*> validP2s;
        for (EmeraldPiece* piece : piece2List)
        {
            Vector3f p2Loc(piece->getPosition());
            Vector3f diff = p1Loc - p2Loc;
            float dist = diff.lengthSquared();

            if (dist >= minDist)
            {
                validP2s.push_back(piece);
            }
        }

        //Pick a random p2
        EmeraldManager::piece2 = validP2s[(int)(Maths::random()*validP2s.size())];
        Vector3f p2Loc(EmeraldManager::piece2->getPosition());
        Vector3f diff = p1Loc - p2Loc;

        //Calculate cross products to p3's
        std::vector<float> crossProducts;
        for (EmeraldPiece* piece : piece3List)
        {
            Vector3f p1ToP3 = p1Loc - piece->getPosition();
            float cp = (diff.cross(&p1ToP3)).lengthSquared();

            crossProducts.push_back(cp);
        }

        //Get the minimum area needed to be a valid p3
        std::sort(crossProducts.begin(), crossProducts.end());
        float minCP = crossProducts[crossProducts.size()/2];

        //Generate valid p3's
        std::vector<EmeraldPiece*> validP3s;
        for (EmeraldPiece* piece : piece3List)
        {
            Vector3f p1ToP3 = p1Loc - piece->getPosition();
            float cp = (diff.cross(&p1ToP3)).lengthSquared();

            if (cp >= minCP)
            {
                validP3s.push_back(piece);
            }
        }

        //Pick a random p3
        EmeraldManager::piece3 = validP3s[(int)(Maths::random()*validP3s.size())];

        EmeraldManager::piecesRemaining = 3;

        //Delete all the other pieces
        for (EmeraldPiece* piece : piece1List)
        {
            if (piece != EmeraldManager::piece1)
            {
                Global::deleteEntity(piece);
            }
        }
        for (EmeraldPiece* piece : piece2List)
        {
            if (piece != EmeraldManager::piece2)
            {
                Global::deleteEntity(piece);
            }
        }
        for (EmeraldPiece* piece : piece3List)
        {
            if (piece != EmeraldManager::piece3)
            {
                Global::deleteEntity(piece);
            }
        }
    }
    else //Ring or chao mode, delete all emerald pieces
    {
        for (Entity* e : gameEntitiesToAdd)
        {
            if (e->isEmeraldPiece()) 
            {
                EmeraldPiece* piece = (EmeraldPiece*)e;
                Global::deleteEntity(piece);
            }
        }

        EmeraldManager::radar1->setVisible(false);
        EmeraldManager::radar2->setVisible(false);
        EmeraldManager::radar3->setVisible(false);
    }
}

void EmeraldManager::updatePiece(EmeraldPiece* piece, float* pingTimer, GuiTexture* radar, EmeraldPiece* closestPiece)
{
    extern float dt;

    if (piece == nullptr)
    {
        radar->setVisible(false);
        return;
    }
    radar->setTexture(EmeraldManager::radarGreyId);
    radar->setScale(1);

    float distToPiece =  (piece->position - Global::gameMainPlayer->position).length();

    *pingTimer = *pingTimer - dt;

    if (distToPiece >= 1000)
    {
        *pingTimer  = 1.0f;
    }

    if (*pingTimer <= 0.0f && piece == closestPiece)
    {
        //Play the sound in the direction to the piece from the camera
        Vector3f diff = piece->position - Global::gameCamera->eye;
        diff.scale(0.25f);

        Vector3f pingPosition = Global::gameCamera->eye + diff;

        AudioPlayer::play(34, &pingPosition);
    }

    if (distToPiece >= 600 && distToPiece < 1000)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer += 1.0f;
        }
        radar->setTexture(EmeraldManager::radarBlueId);
        radar->setScale(1 + 0.5f*(*pingTimer));
    }
    else if (distToPiece >= 200 && distToPiece < 600)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer = 0.5f;
        }
        radar->setTexture(EmeraldManager::radarGreenId);
        radar->setScale(1 + 0.5f*(*pingTimer/0.5f));
    }
    else if (distToPiece >= 50 && distToPiece < 200)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer = 0.25f;
        }
        radar->setTexture(EmeraldManager::radarYellowId);
        radar->setScale(1 + 0.5f*(*pingTimer/0.25f));
    }
    else if (distToPiece >= 0 && distToPiece < 50)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer = 0.2f;
        }
        radar->setTexture(EmeraldManager::radarRedId);
        radar->setScale(1 + 0.5f*(*pingTimer/0.2f));
    }
}

void EmeraldManager::step()
{
    if (Global::gameIsChaoMode || Global::gameIsRingMode)
    {
        return;
    }

    EmeraldPiece* closestPiece = EmeraldManager::getClosestPiece();
    EmeraldManager::updatePiece(EmeraldManager::piece1, &EmeraldManager::pingTimer1, EmeraldManager::radar1, closestPiece);
    EmeraldManager::updatePiece(EmeraldManager::piece2, &EmeraldManager::pingTimer2, EmeraldManager::radar2, closestPiece);
    EmeraldManager::updatePiece(EmeraldManager::piece3, &EmeraldManager::pingTimer3, EmeraldManager::radar3, closestPiece);

    GuiManager::addGuiToRender(EmeraldManager::radar1);
    GuiManager::addGuiToRender(EmeraldManager::radar2);
    GuiManager::addGuiToRender(EmeraldManager::radar3);

    if (closestPiece != nullptr)
    {
        Vector3f diff = Global::gameMainPlayer->getCenterPosition() - closestPiece->position;
        if (diff.lengthSquared() < 25*25)
        {
            Vector3f pos = Global::gameMainPlayer->getCenterPosition();
            pos.y += 15;
            Vector2f screenPos = Maths::calcScreenCoordsOfWorldPoint(&pos);
            EmeraldManager::nearPiece->getPosition()->set(&screenPos);
            GuiManager::addGuiToRender(EmeraldManager::nearPiece);
        }
    }
}

EmeraldPiece* EmeraldManager::getClosestPiece()
{
    EmeraldPiece* pieceToReturn = nullptr;
    float closestDist = 1000000000.0f;

    if (EmeraldManager::piece1 != nullptr)
    {
        float distToPiece =  (EmeraldManager::piece1->position - Global::gameMainPlayer->position).length();
        if (distToPiece < closestDist)
        {
            pieceToReturn = EmeraldManager::piece1;
            closestDist = distToPiece;
        }
    }
    if (EmeraldManager::piece2 != nullptr)
    {
        float distToPiece =  (EmeraldManager::piece2->position - Global::gameMainPlayer->position).length();
        if (distToPiece < closestDist)
        {
            pieceToReturn = EmeraldManager::piece2;
            closestDist = distToPiece;
        }
    }
    if (EmeraldManager::piece3 != nullptr)
    {
        float distToPiece =  (EmeraldManager::piece3->position - Global::gameMainPlayer->position).length();
        if (distToPiece < closestDist)
        {
            pieceToReturn = EmeraldManager::piece3;
            closestDist = distToPiece;
        }
    }
    return pieceToReturn;
}

void EmeraldManager::collectPiece(EmeraldPiece* collectedPiece)
{
    if (collectedPiece == EmeraldManager::piece1)
    {
        EmeraldManager::piece1 = nullptr;
    }
    else if (collectedPiece == EmeraldManager::piece2)
    {
        EmeraldManager::piece2 = nullptr;
    }
    else if (collectedPiece == EmeraldManager::piece3)
    {
        EmeraldManager::piece3 = nullptr;
    }

    EmeraldManager::piecesRemaining--;

    if (EmeraldManager::piecesRemaining <= 0)
    {
        Global::mainHudTimer->frozen = true;

        Global::finishStageTimer = 0;
    }
}
