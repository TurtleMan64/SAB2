#include "emeraldmanager.hpp"
#include "entity.hpp"
#include "emeraldpiece.hpp"
#include "../engineTester/main.hpp"
#include "../guis/guimanager.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/vector.hpp"
#include "../entities/camera.hpp"
#include "../guis/guirenderer.hpp"
#include "../guis/guiimageresources.hpp"
#include "../guis/guiimage.hpp"
#include "../menu/timer.hpp"
#include "../loading/loadergl.hpp"
#include "../renderEngine/display.hpp"

#include <vector>
#include <list>
#include <unordered_map>
#include <algorithm>

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

GuiImage* EmeraldManager::radar1    = nullptr;
GuiImage* EmeraldManager::radar2    = nullptr;
GuiImage* EmeraldManager::radar3    = nullptr;
GuiImage* EmeraldManager::nearPiece = nullptr;

EmeraldManager::~EmeraldManager()
{
    if (EmeraldManager::radar1 != nullptr)
    {
        delete EmeraldManager::radar1; INCR_DEL("GuiImage");
        EmeraldManager::radar1 = nullptr;
    }
    if (EmeraldManager::radar2 != nullptr)
    {
        delete EmeraldManager::radar2; INCR_DEL("GuiImage");
        EmeraldManager::radar2 = nullptr;
    }
    if (EmeraldManager::radar3 != nullptr)
    {
        delete EmeraldManager::radar3; INCR_DEL("GuiImage");
        EmeraldManager::radar3 = nullptr;
    }
    if (EmeraldManager::nearPiece != nullptr)
    {
        delete EmeraldManager::nearPiece; INCR_DEL("GuiImage");
        EmeraldManager::nearPiece = nullptr;
    }

    LoaderGL::deleteTexture(EmeraldManager::radarGreyId);
    LoaderGL::deleteTexture(EmeraldManager::radarBlueId);
    LoaderGL::deleteTexture(EmeraldManager::radarGreenId);
    LoaderGL::deleteTexture(EmeraldManager::radarYellowId);
    LoaderGL::deleteTexture(EmeraldManager::radarRedId);
    LoaderGL::deleteTexture(EmeraldManager::nearPieceId);
    
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
    EmeraldManager::radarGreyId   = LoaderGL::loadTexture("res/Images/TreasureHunting/RadarGrey.png");
    EmeraldManager::radarBlueId   = LoaderGL::loadTexture("res/Images/TreasureHunting/RadarBlue.png");
    EmeraldManager::radarGreenId  = LoaderGL::loadTexture("res/Images/TreasureHunting/RadarGreen.png");
    EmeraldManager::radarYellowId = LoaderGL::loadTexture("res/Images/TreasureHunting/RadarYellow.png");
    EmeraldManager::radarRedId    = LoaderGL::loadTexture("res/Images/TreasureHunting/RadarRed.png");
    EmeraldManager::nearPieceId   = LoaderGL::loadTexture("res/Images/TreasureHunting/NearPiece.png");

    float r = Display::ASPECT_RATIO; //screen ratio
    float s = 0.05f; //size of radar tiles

    //Create Radar gui textures
    EmeraldManager::radar1 = new GuiImage(EmeraldManager::radarGreyId, 0.5f-((1.5f*s)/r), 1-s, s/r, s, 0); INCR_NEW("GuiImage");
    EmeraldManager::radar2 = new GuiImage(EmeraldManager::radarGreyId, 0.5f,              1-s, s/r, s, 0); INCR_NEW("GuiImage");
    EmeraldManager::radar3 = new GuiImage(EmeraldManager::radarGreyId, 0.5f+((1.5f*s)/r), 1-s, s/r, s, 0); INCR_NEW("GuiImage");

    EmeraldManager::nearPiece = new GuiImage(EmeraldManager::nearPieceId, 0, 0, 0.1f/r, 0.1f, 0); INCR_NEW("GuiImage");

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

        EmeraldManager::radar1->visible = false;
        EmeraldManager::radar2->visible = false;
        EmeraldManager::radar3->visible = false;
    }
    else if (Global::gameIsNormalMode)
    {
        if (piece1List.size() <= 0)
        {
            printf("Error: No Emerald Piece 1's exist to pick from.\n");
        }
        if (piece2List.size() <= 0)
        {
            printf("Error: No Emerald Piece 2's exist to pick from.\n");
        }
        if (piece3List.size() <= 0)
        {
            printf("Error: No Emerald Piece 3's exist to pick from.\n");
        }

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

        EmeraldManager::radar1->visible = false;
        EmeraldManager::radar2->visible = false;
        EmeraldManager::radar3->visible = false;
    }
}

void EmeraldManager::updatePiece(EmeraldPiece* piece, float* pingTimer, GuiImage* radar, EmeraldPiece* closestPiece)
{
    extern float dt;

    if (piece == nullptr)
    {
        radar->visible = false;
        return;
    }
    radar->textureId = EmeraldManager::radarGreyId;
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
        radar->textureId = EmeraldManager::radarBlueId;
        radar->setScale(1 + 0.5f*(*pingTimer));
    }
    else if (distToPiece >= 200 && distToPiece < 600)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer = 0.5f;
        }
        radar->textureId = EmeraldManager::radarGreenId;
        radar->setScale(1 + 0.5f*(*pingTimer/0.5f));
    }
    else if (distToPiece >= 50 && distToPiece < 200)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer = 0.25f;
        }
        radar->textureId = EmeraldManager::radarYellowId;
        radar->setScale(1 + 0.5f*(*pingTimer/0.25f));
    }
    else if (distToPiece >= 0 && distToPiece < 50)
    {
        if (*pingTimer <= 0)
        {
            *pingTimer = 0.2f;
        }
        radar->textureId = EmeraldManager::radarRedId;
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

    GuiManager::addImageToRender(EmeraldManager::radar1);
    GuiManager::addImageToRender(EmeraldManager::radar2);
    GuiManager::addImageToRender(EmeraldManager::radar3);

    if (closestPiece != nullptr)
    {
        Vector3f diff = Global::gameMainPlayer->getCenterPosition() - closestPiece->position;
        if (diff.lengthSquared() < 25*25)
        {
            Vector3f pos = Global::gameMainPlayer->getCenterPosition();
            pos.y += 15;
            Vector2f screenPos = Maths::calcScreenCoordsOfWorldPoint(&pos);
            EmeraldManager::nearPiece->getPosition()->set(&screenPos);
            GuiManager::addImageToRender(EmeraldManager::nearPiece);
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
