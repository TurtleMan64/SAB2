#ifndef LOSTCHAO_H
#define LOSTCHAO_H

class TexturedModel;
class Source;

#include <list>
#include "entity.hpp"

class LostChao : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    Source* sourceCry = nullptr;

public:
    LostChao();
    LostChao(float x, float y, float z, float rot);
    ~LostChao();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
