#ifndef SHIELDGREEN_H
#define SHIELDGREEN_H

class TexturedModel;

#include <list>
#include "entity.h"

class ShieldGreen : public Entity
{
public:
    ShieldGreen();

    void step();

    std::list<TexturedModel*>* getModels();
};
#endif
