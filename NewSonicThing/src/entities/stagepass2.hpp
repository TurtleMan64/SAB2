#ifndef STAGEPASS2_H
#define STAGEPASS2_H

class TexturedModel;

#include <list>
#include "entity.hpp"


class StagePass2 : public Entity
{
private:
    static std::vector<TexturedModel*> models;

public:
    StagePass2();
    StagePass2(const char* objFolder, const char* objFilename);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void deleteStaticModels();
};
#endif
