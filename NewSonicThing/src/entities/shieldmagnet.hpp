#ifndef SHIELDMAGNET_H
#define SHIELDMAGNET_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class ShieldMagnet : public Entity
{
public:
    ShieldMagnet();

    void step();

    std::vector<TexturedModel*>* getModels();
};
#endif
