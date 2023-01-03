#ifndef SH_STONEBLOCK_H
#define SH_STONEBLOCK_H

#include <list>
#include <vector>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;
class Dummy;

class SH_StoneBlock : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static std::vector<TexturedModel*> modelsP1;
    static std::vector<TexturedModel*> modelsP2;
    static std::vector<TexturedModel*> modelsP3;
    static std::vector<TexturedModel*> modelsP4;
    static std::vector<TexturedModel*> modelsP5;
    static std::vector<TexturedModel*> modelsP6;
    static std::vector<TexturedModel*> modelsP7;
    static std::vector<TexturedModel*> modelsP8;
    static std::vector<TexturedModel*> modelsP9;
    static std::vector<TexturedModel*> modelsP10;
    static std::vector<TexturedModel*> modelsP11;
    static std::vector<TexturedModel*> modelsP12;

    static CollisionModel* cmOriginal;

    float breakTimer = 0.75f;
    std::vector<Dummy*> pieces;
    std::vector<Vector3f> piecesVel;

public:
    SH_StoneBlock(float x, float y, float z, float rot);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
