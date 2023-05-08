#ifndef PC_DOOR_H
#define PC_DOOR_H

class TexturedModel;

#include <vector>
#include "../entity.hpp"
#include "../collideableobject.hpp"

class PC_Door : public CollideableObject
{
private:
    static std::vector<TexturedModel*> models;
    static CollisionModel* cmOriginal;

public:
    int id = -1;
    float moveTimer = -1.0f;

    PC_Door();
    PC_Door(float x, float y, float z, float rotY, int id);

    void step();

    std::vector<TexturedModel*>* getModels();

    void activateFromSwitch(int switchId);

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
