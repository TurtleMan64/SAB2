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
    static std::list<TexturedModel*> models;
    static std::list<TexturedModel*> modelsP1;
    static std::list<TexturedModel*> modelsP2;
    static std::list<TexturedModel*> modelsP3;
    static std::list<TexturedModel*> modelsP4;
    static std::list<TexturedModel*> modelsP5;
    static std::list<TexturedModel*> modelsP6;
    static std::list<TexturedModel*> modelsP7;
    static std::list<TexturedModel*> modelsP8;
    static std::list<TexturedModel*> modelsP9;
    static std::list<TexturedModel*> modelsP10;
    static std::list<TexturedModel*> modelsP11;
    static std::list<TexturedModel*> modelsP12;

    static CollisionModel* cmOriginal;

    float breakTimer = 0.75f;
    std::vector<Dummy*> pieces;
    std::vector<Vector3f> piecesVel;

public:
    SH_StoneBlock(float x, float y, float z, float rot);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
