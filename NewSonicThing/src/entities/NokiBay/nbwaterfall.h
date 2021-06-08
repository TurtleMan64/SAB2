#ifndef NBWATERFALL_H
#define NBWATERFALL_H

class TexturedModel;

#include <list>
#include "../entity.h"

class NB_Waterfall : public Entity
{
private:
    static std::list<TexturedModel*> models;

public:
    NB_Waterfall();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
