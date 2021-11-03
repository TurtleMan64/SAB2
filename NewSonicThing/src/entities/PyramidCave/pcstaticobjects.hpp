#ifndef PC_STATICOBJECTS
#define PC_STATICOBJECTS

#include <list>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class TexturedModel;
class Body;
class CollisionModel;

class PC_StaticObjects : public CollideableObject
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

    //Body used to be able to display the vertex colors model
    Body* modelVertexColors = nullptr;
    //Body used to be able to display the transparent model
    Body* modelTransparent = nullptr;

    //Functions for the constructor start here
    
    //Makes the body used for the Vertex Colors model
    void setupModelVertexColors();
    //Makes the body used for the transparent model
    void setupModelTransparent();

public:
    PC_StaticObjects();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
