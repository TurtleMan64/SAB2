#ifndef WATERZONE_H
#define WATERZONE_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class WaterZone : public Entity
{
private:
    Vector3f width;
    
public:
    WaterZone();
    WaterZone(float x,  float y,  float z, 
              float xw, float yw, float zw);

    void step();

    std::vector<TexturedModel*>* getModels();
};
#endif
