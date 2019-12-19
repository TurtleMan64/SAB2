#ifndef SKYSPHERE_H
#define SKYSPHERE_H

class TexturedModel;

#include <list>
#include "entity.h"
#include <string>


class SkySphere : public Entity
{
private:
    static std::list<TexturedModel*> models;

    bool followsY = false;

public:
    SkySphere();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadModels(char* folder, char* objname, char* mtlname);

    static void deleteModels();

    void setFollowsY(bool newFollowsY);

    bool getFollowsY();
};
#endif