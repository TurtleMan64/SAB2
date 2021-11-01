#ifndef FAKETEXTURE_H
#define FAKETEXTURE_H

#include <string>

class FakeTexture
{
public:
    std::string name;
    char type = 0;
    char sound = 0;
    char particle = 0;

    FakeTexture();
};
#endif
