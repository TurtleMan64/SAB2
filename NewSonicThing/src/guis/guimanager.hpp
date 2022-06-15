#ifndef GUIMANAGER_H
#define GUIMANAGER_H

class FontType;
class GUIText;
class GUINumber;
class GuiImage;

#include <list>

class GuiManager
{
private:
    static std::list<GuiImage*> guisToRender;

public:
    static void init();

    static void refresh();

    static void addImageToRender(GuiImage* newImage);

    static void removeImageToRender(GuiImage* imageToRemove);

    static void clearGuisToRender();
};

#endif
