#ifndef MH_MISSLE
#define MH_MISSLE

#include <list>
#include "../entity.h"

class TexturedModel;

class MH_Missle : public Entity
{
private:
    static std::list<TexturedModel*> models;

    float timer = 0.0f;

    void putPlayerAtHandle();
    
public:
    MH_Missle();

    MH_Missle(float x, float y, float z);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
