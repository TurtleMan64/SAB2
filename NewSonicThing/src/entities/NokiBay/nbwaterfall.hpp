#ifndef NBWATERFALL_H
#define NBWATERFALL_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class NB_Waterfall : public Entity
{
private:
    static std::vector<TexturedModel*> models;

public:
    NB_Waterfall();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
