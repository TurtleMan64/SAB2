#ifndef RACEGHOST_H
#define RACEGHOST_H

class TexturedModel;
class PlayerModel;

#include <list>
#include "entity.h"

class GhostFrame
{
public:
    float time;
    int animIndex;
    float animTime;
    Vector3f pos;
    Vector4f rot;
    Vector3f up;

    GhostFrame();

    GhostFrame(float time, int animIndex, float animTime, Vector3f* pos, Vector4f* rot, Vector3f* up);

    GhostFrame(GhostFrame* other);

    std::string toString();

    static GhostFrame interpolate(GhostFrame* frameBefore, GhostFrame* frameAfter, float time);
};

class RaceGhost : public Entity
{
private:
    PlayerModel* myModel = nullptr;

    int ghostType; //0 = player ghosts. 1 = mighty race
    std::vector<GhostFrame> frames;
    float averageFramesPerSecond;

public:
    RaceGhost();
    RaceGhost(const char* filePath, int missionNumber);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
