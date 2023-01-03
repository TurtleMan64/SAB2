#ifndef BODY_H
#define BODY_H

class Vector3f;
class TexturedModel;

#include <list>
#include <vector>
#include "../entities/entity.hpp"
#include "animation.hpp"


class Body : public Entity
{
private:
    float baseX;
    float baseY;
    float baseZ;
    float baseRotY;
    float baseRotZ;
    float baseRotX;
    float baseRotS;
    float baseScale;
    float prevTime;

    std::vector<TexturedModel*>* myModels;

public:
    int animationIndex;
    float time;
    float deltaTime;

    std::vector<Animation>* animations;

    Body(std::vector<TexturedModel*>* models);

    void step();

    void update(float time);

    void setBaseOrientation(Vector3f* basePosition, float rotX, float rotY, float rotZ, float rotS, float scale);

    void setBaseOrientation(float baseX, float baseY, float baseZ, float rotX, float rotY, float rotZ, float rotS, float scale);

    inline std::vector<TexturedModel*>* getModels();

    inline void setModels(std::vector<TexturedModel*>* newModels);
};
#endif
