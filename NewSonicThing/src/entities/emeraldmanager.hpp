#ifndef EMERALDMANAGER_H
#define EMERALDMANAGER_H

class EmeraldPiece;
class GuiImage;

#include "entity.hpp"
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

    static GLuint radarGreyId;
    static GLuint radarBlueId;
    static GLuint radarGreenId;
    static GLuint radarYellowId;
    static GLuint radarRedId;
    static GLuint nearPieceId;

    static GuiImage* radar1;
    static GuiImage* radar2;
    static GuiImage* radar3;
    static GuiImage* nearPiece;

    static void updatePiece(EmeraldPiece* piece, float* pingTimer, GuiImage* radar, EmeraldPiece* closestPiece);

public:
    EmeraldManager();
    ~EmeraldManager();

    void step();
    
    static void collectPiece(EmeraldPiece* collectedPiece);

    static EmeraldPiece* getClosestPiece();
};
#endif
