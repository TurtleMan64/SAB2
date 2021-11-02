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

    std::list<TexturedModel*>* getModels();
};
#endif
