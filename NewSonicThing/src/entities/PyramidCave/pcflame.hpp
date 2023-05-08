#ifndef PC_FLAME
#define PC_FLAME

#include <vector>
#include "../entity.hpp"

class TexturedModel;
class Source;

class PC_Flame : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    Source* sourceFire = nullptr;

public:
    PC_Flame(float x, float y, float z, float scale);
    ~PC_Flame();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
