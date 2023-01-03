#ifndef SHIELDGREEN_H
#define SHIELDGREEN_H

class TexturedModel;

#include <list>
#include "entity.hpp"

class ShieldGreen : public Entity
{
public:
    ShieldGreen();

    void step();

    std::vector<TexturedModel*>* getModels();
};
#endif
