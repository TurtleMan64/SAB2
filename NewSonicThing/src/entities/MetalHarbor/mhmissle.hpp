#ifndef MH_MISSLE
#define MH_MISSLE

#include <list>
#include "../entity.hpp"

class TexturedModel;

class MH_Missle : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    float timer = 0.0f;

    void putPlayerAtHandle();
    
public:
    MH_Missle();

    MH_Missle(float x, float y, float z);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
