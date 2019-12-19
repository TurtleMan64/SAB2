#ifndef EXTRAS_H
#define EXTRAS_H

class GUIText;

#include <glad/glad.h>
#include "menu.h"

class Extras : public Menu
{
private:
    GUIText* textTime = nullptr;
    GUIText* timeBro = nullptr;
    bool selectInputPrevious;
    bool backInputPrevious;

public:
    Extras();
    ~Extras();
    Menu* step();
};

#endif
