#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H

class EntityShader;
class TexturedModel;
class Entity;
class Matrix4f;

#include <unordered_map>
#include <vector>

class EntityRenderer
{
private:
    float clockTime = 0.0f;

    EntityShader* shader = nullptr;

    void prepareTexturedModel(TexturedModel* model);

    void unbindTexturedModel();

    void prepareInstance(Entity* entity);

public:
    EntityRenderer(EntityShader* shader, Matrix4f* projectionMatrix);

    void render(Entity* entity);

    void renderNEW(std::unordered_map<TexturedModel*, std::vector<Entity*>>* entities);

    void updateProjectionMatrix(Matrix4f* projectionMatrix);
};
#endif
