#ifndef STAGEPASS3_H
#define STAGEPASS3_H

class TexturedModel;

#include <list>
#include "entity.hpp"


class StagePass3 : public Entity
{
private:
    static std::vector<TexturedModel*> models;

public:
    StagePass3();
    StagePass3(const char* objFolder, const char* objFilename);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void deleteStaticModels();
};
#endif