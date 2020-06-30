#ifndef TC_DASH
#define TC_DASH

class TexturedModel;
class Triangle3D;

#include <list>
#include "../entity.h"
#include "../../toolbox/vector.h"

class TC_Dash : public Entity
{
private:
    static std::list<TexturedModel*> models;

    bool playerIsIn = false;

public:
    TC_Dash();
    TC_Dash(
        float x, float y, float z,
        float atX, float atY, float atZ,
        float normX, float normY, float normZ);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
