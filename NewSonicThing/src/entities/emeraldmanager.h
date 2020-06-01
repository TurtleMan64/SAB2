#ifndef EMERALDMANAGER_H
#define EMERALDMANAGER_H

class EmeraldPiece;
class GuiTexture;

#include "entity.h"
#include <glad/glad.h>

class EmeraldManager : public Entity
{
private:
    static EmeraldPiece* piece1;
    static EmeraldPiece* piece2;
    static EmeraldPiece* piece3;
    static int piecesRemaining;
    static float pingTimer1;
    static float pingTimer2;
    static float pingTimer3;

    static GLuint radarGreyID;
    static GLuint radarBlueID;
    static GLuint radarGreenID;
    static GLuint radarYellowID;
    static GLuint radarRedID;
    static GLuint nearPieceID;

    static GuiTexture* radar1;
    static GuiTexture* radar2;
    static GuiTexture* radar3;
    static GuiTexture* nearPiece;

    static void updatePiece(EmeraldPiece* piece, float* pingTimer, GuiTexture* radar, EmeraldPiece* closestPiece);

public:
    EmeraldManager();
    ~EmeraldManager();

    void step();
    
    static void collectPiece(EmeraldPiece* collectedPiece);

    static EmeraldPiece* getClosestPiece();
};
#endif
