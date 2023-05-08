#ifndef PC_NEONSIGN
#define PC_NEONSIGN

#include <vector>
#include "../entity.hpp"

class TexturedModel;

class PC_NeonSign : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    float spinSpeed = 0.0f;

public:
    PC_NeonSign(float x, float y, float z);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
