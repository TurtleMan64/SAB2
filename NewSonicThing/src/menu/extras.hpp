#ifndef EXTRAS_H
#define EXTRAS_H

class GUIText;

#include <glad/glad.h>
#include "menu.hpp"

class Extras : public Menu
{
private:
    GUIText* textTime = nullptr;
    GUIText* timeBro = nullptr;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

public:
    Extras();
    ~Extras();
    Menu* step();
    void draw() {}
    void setVisible(bool) {}
};

#endif
