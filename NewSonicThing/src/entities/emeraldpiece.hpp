#ifndef EMERALDPIECE_H
#define EMERALDPIECE_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class EmeraldPiece : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    static float baseScale;

    static float lastPieceCollectedTimestamp;
    
    int pieceNumber; //1, 2 or 3

    bool hardModePiece;

    float collectTimer;
    const float collectTimerMax = 2.0f;

    bool isDiggable;

public:
    Vector3f hintCameraCenter;

    EmeraldPiece();
    EmeraldPiece(
        float x, float y, float z,
        int pieceNumber, int isDiggable,
        int hardModePiece);

    void step();

    bool isEmeraldPiece() const;

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
    
    int getPieceNumber();

    Vector3f getHintLocation();

    bool isHardModePiece();
};
#endif
