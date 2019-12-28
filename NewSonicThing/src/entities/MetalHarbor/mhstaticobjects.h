#ifndef MH_STATICOBJECTS
#define MH_STATICOBJECTS

#include <list>
#include "../entity.h"
#include "../collideableobject.h"

class TexturedModel;
class Body;

class MH_StaticObjects : public CollideableObject
{
private:
    //The visual model that doesn't use vertex colours
    static std::list<TexturedModel*> models;
    //The visual model that uses vertex colours
    static std::list<TexturedModel*> modelsVertexColours;
    //The visual model that is for textures with transparency
    static std::list<TexturedModel*> modelsTransparent;
    //The collision model
    static CollisionModel* cmOriginal;

    //Body used to be able to display the vertex colours model
    Body* modelVertexColours = nullptr;
    //Body used to be able to display the transparent model
    Body* modelTransparent = nullptr;

    //Functions for the constructor start here
    
    //Makes the body used for the Vertex Colours model
    void setupModelVertexColours();
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
