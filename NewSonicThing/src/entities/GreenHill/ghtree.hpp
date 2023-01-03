#ifndef GHTREE_H
#define GHTREE_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class GH_Tree : public Entity
{
private:
    static std::vector<TexturedModel*> models;

public:
    GH_Tree();
    GH_Tree(float x, float y, float z, float rotY, float scale);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
