#ifndef FONTRENDERER_H
#define FONTRENDERER_H

class FontType;
class GUIText;
class GUINumber;
class FontShader;

#include <unordered_map>
#include <list>

class FontRenderer
{
private:
    FontShader* shader;

    void prepare();

    void renderText(GUIText* text);

    void renderNumber(GUINumber* number);

    void endRendering();

public:
    FontRenderer();

    void render(
        std::unordered_map<FontType*, std::list<GUIText*>>* texts, 
        std::unordered_map<FontType*, std::list<GUINumber*>>* numbers);

    void cleanUp();
};

#endif