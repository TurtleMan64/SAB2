#ifndef TC_DASH
#define TC_DASH

class TexturedModel;
class Triangle3D;

#include <vector>
#include "../entity.hpp"
#include "../../toolbox/vector.hpp"

class TC_Dash : public Entity
{
private:
    static std::vector<TexturedModel*> models;

    bool playerIsIn = false;

public:
    TC_Dash();
    TC_Dash(
        float x, float y, float z,
        float atX, float atY, float atZ,
        float normX, float normY, float normZ);

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
