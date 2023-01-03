#ifndef DL_PLANT1
#define DL_PLANT1

#include <list>
#include "../entity.hpp"

class TexturedModel;

class DL_Plant1 : public Entity
{
private:
    static std::vector<TexturedModel*> models;

public:
    DL_Plant1();

    DL_Plant1(
        float x, float y, float z,
        float rotX, float rotY, float rotZ,
        float scale);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
