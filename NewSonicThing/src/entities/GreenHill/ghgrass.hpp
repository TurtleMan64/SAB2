#ifndef GHGRASS_H
#define GHGRASS_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class GH_Grass : public Entity
{
private:
    static std::list<TexturedModel*> models;

public:
    GH_Grass();
    GH_Grass(float x, float y, float z, float rotY, float scale);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
