#ifndef DL_TREE
#define DL_TREE

#include <list>
#include "../entity.h"

class TexturedModel;

class DL_Tree : public Entity
{
private:
    static std::list<TexturedModel*> models;

public:
    DL_Tree();

    DL_Tree(
        float x, float y, float z,
        float rotX, float rotY, float rotZ,
        float scale);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
