#ifndef STAGETRANSPARENT_H
#define STAGETRANSPARENT_H

class TexturedModel;

#include <list>
#include "entity.hpp"


class StageTransparent : public Entity
{
private:
    static std::vector<TexturedModel*> models;

public:
    StageTransparent();
    StageTransparent(const char* objFolder, const char* objFilename);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void deleteStaticModels();
};
#endif
