#ifndef SKYSPHERE_H
#define SKYSPHERE_H

class TexturedModel;

#include <list>
#include "entity.hpp"
#include <string>


class SkySphere : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    bool followsY = false;

public:
    SkySphere();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadModels(char* folder, char* objname, char* mtlname);

    static void deleteModels();

    void setFollowsY(bool newFollowsY);

    bool getFollowsY();
};
#endif