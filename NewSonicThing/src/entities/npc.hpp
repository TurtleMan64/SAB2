#ifndef NPC_H
#define NPC_H

class TexturedModel;
class GUIText;
class Dummy;

#include <list>
#include "entity.hpp"
#include <string>

class NPC : public Entity
{
private:
    static std::vector<TexturedModel*> models;
    static std::vector<TexturedModel*> modelsGlasses;
    static GUIText* messageGUI1;
    static GUIText* messageGUI2;

    Dummy* glasses = nullptr;
    std::string myMessage1;
    std::string myMessage2;
    bool found;
    int id;

public:
    NPC();
    ~NPC();
    NPC(float x, float y, float z, float dirX, float dirZ, int id, std::string message);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
