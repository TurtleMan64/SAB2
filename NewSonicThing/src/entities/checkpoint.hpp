#ifndef CHECKPOINT_H
#define CHECKPOINT_H

class TexturedModel;
class Dummy;

#include <list>
#include "entity.hpp"
#include <AL/al.h>


class Checkpoint : public Entity
{
private:
    static std::vector<TexturedModel*> modelsBase;
    static std::vector<TexturedModel*> modelsBall;

    Dummy* ball1 = nullptr;
    Dummy* ball2 = nullptr;
    static float colHorizontal;
    static float colVertical;

    bool isHit;

    static ALuint savedBGMIntro;
    static ALuint savedBGMLoop;

public:
    Checkpoint();
    Checkpoint(float x, float y, float z, float rotY);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
