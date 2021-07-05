#ifndef WATERZONE_H
#define WATERZONE_H

class TexturedModel;

#include <list>
#include "entity.h"

class WaterZone : public Entity
{
private:
    Vector3f width;
    
public:
    WaterZone();
    WaterZone(float x,  float y,  float z, 
              float xw, float yw, float zw);

    void step();

    std::list<TexturedModel*>* getModels();
};
#endif
