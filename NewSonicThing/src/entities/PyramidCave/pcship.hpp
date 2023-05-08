#ifndef PC_SHIP
#define PC_SHIP

#include <vector>
#include "../entity.hpp"

class TexturedModel;

class PC_Ship : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    float timer = 0.0f;

public:
    PC_Ship(float x, float y, float z, float rot);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
