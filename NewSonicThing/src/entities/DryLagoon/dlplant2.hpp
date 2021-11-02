#ifndef DL_PLANT2
#define DL_PLANT2

#include <list>
#include "../entity.hpp"

class TexturedModel;

class DL_Plant2 : public Entity
{
private:
    static std::list<TexturedModel*> models;

public:
    DL_Plant2();

    DL_Plant2(
        float x, float y, float z,
        float rotX, float rotY, float rotZ,
        float scale);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
