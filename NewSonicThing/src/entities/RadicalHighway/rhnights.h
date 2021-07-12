#ifndef RH_NIGHTS
#define RH_NIGHTS

#include <list>
#include "../entity.h"

class TexturedModel;

class RH_Nights : public Entity
{
private:
    static std::list<TexturedModel*> models;

    float timer = 0.0f;

public:
    RH_Nights();

    RH_Nights(float x, float y, float z);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif