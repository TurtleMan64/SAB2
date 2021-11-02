#ifndef DPBIGSHINE_H
#define DPBIGSHINE_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class DP_BigShine : public Entity
{
private:
    static std::list<TexturedModel*> models;

public:
    DP_BigShine();
    DP_BigShine(float x, float y, float z);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
