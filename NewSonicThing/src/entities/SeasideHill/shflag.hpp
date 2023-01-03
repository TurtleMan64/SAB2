#ifndef SH_FLAG_H
#define SH_FLAG_H

#include <list>
#include <vector>
#include "../entity.hpp"

class TexturedModel;

class SH_Flag : public Entity
{
private:
    static std::vector<TexturedModel*> models0;
    static std::vector<TexturedModel*> models1;
    static std::vector<TexturedModel*> models2;
    static std::vector<TexturedModel*> models3;
    static std::vector<TexturedModel*> models4;
    static std::vector<TexturedModel*> models5;

    float timer = 0.0f;

public:
    SH_Flag(float x, float y, float z, float scale);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
