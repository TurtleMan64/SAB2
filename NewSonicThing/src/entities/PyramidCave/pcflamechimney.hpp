#ifndef PC_FLAMECHIMNEY
#define PC_FLAMECHIMNEY

#include <vector>
#include "../entity.hpp"

class TexturedModel;

class PC_FlameChimney : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    float animationTime = 0.0f;
    float waitTime = 0.0f;

public:
    PC_FlameChimney(float x, float y, float z);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
