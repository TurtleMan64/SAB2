#ifndef SH_FLAG_H
#define SH_FLAG_H

#include <list>
#include <vector>
#include "../entity.hpp"

class TexturedModel;

class SH_Flag : public Entity
{
private:
    static std::list<TexturedModel*> models0;
    static std::list<TexturedModel*> models1;
    static std::list<TexturedModel*> models2;
    static std::list<TexturedModel*> models3;
    static std::list<TexturedModel*> models4;
    static std::list<TexturedModel*> models5;

    float timer = 0.0f;

public:
    SH_Flag(float x, float y, float z, float scale);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
