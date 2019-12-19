#ifndef GOALRING_H
#define GOALRING_H

class TexturedModel;
class Dummy;
class Source;

#include <list>
#include "entity.h"

class GoalRing : public Entity
{
private:
    static std::list<TexturedModel*> modelsRing;
    static std::list<TexturedModel*> modelsText;

    Dummy* text = nullptr;
    Source* sourceRing = nullptr;

public:
    GoalRing();
    GoalRing(float x, float y, float z);
    ~GoalRing();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
