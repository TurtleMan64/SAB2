#ifndef HELP_H
#define HELP_H

class TexturedModel;
class GUIText;

#include <list>
#include "entity.hpp"
#include <string>

class Help : public Entity
{
private:
    static std::vector<TexturedModel*> models;
    static GUIText* messageGUI1;
    static GUIText* messageGUI2;

    std::string myMessage1;
    std::string myMessage2;

public:
    Help();
    ~Help();
    Help(float x, float y, float z, std::string message);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
