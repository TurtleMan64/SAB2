#ifndef MH_STATICOBJECTS
#define MH_STATICOBJECTS

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;
class Dummy;

class MH_StaticObjects : public CollideableObject
{
private:
    //The visual model that doesn't use vertex colors
    static std::list<TexturedModel*> models;
    //The visual model that uses vertex colors
    static std::list<TexturedModel*> modelsVertexColors;
    //The visual model that is for textures with transparency
    static std::list<TexturedModel*> modelsTransparent;
    //The collision model
    static CollisionModel* cmOriginal;

    //Dummy used to be able to display the vertex colors model
    Dummy* modelVertexColors = nullptr;
    //Dummy used to be able to display the transparent model
    Dummy* modelTransparent = nullptr;

    //Functions for the constructor start here
    
    //Makes the body used for the Vertex Colors model
    void setupModelVertexColors();
    //Makes the body used for the transparent model
    void setupModelTransparent();

public:
    MH_StaticObjects();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
