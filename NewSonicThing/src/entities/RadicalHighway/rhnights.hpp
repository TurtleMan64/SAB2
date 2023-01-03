#ifndef RH_NIGHTS
#define RH_NIGHTS

#include <list>
#include "../entity.hpp"

class TexturedModel;

class RH_Nights : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    float timer = 0.0f;

public:
    RH_Nights();

    RH_Nights(float x, float y, float z);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
