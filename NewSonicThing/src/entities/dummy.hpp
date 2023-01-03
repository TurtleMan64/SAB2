#ifndef DUMMY_H
#define DUMMY_H

class Vector3f;
class TexturedModel;

#include <list>
#include <vector>
#include "entity.hpp"

class Dummy : public Entity
{
private:
    std::vector<TexturedModel*>* myModels;

public:
    Dummy(std::vector<TexturedModel*>* models);

    void step();

    void setRotation(float xr, float yr, float zr, float sr);

    std::vector<TexturedModel*>* getModels();

    void setModels(std::vector<TexturedModel*>* newModels);
};
#endif
