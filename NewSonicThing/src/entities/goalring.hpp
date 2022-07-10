#ifndef GOALRING_H
#define GOALRING_H

class TexturedModel;
class Source;

#include <list>
#include "entity.hpp"

class GoalRing : public Entity
{
private:
    static std::list<TexturedModel*> models;

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
