#ifndef ENTITIES_H
#define ENTITIES_H

class TexturedModel;
class CollisionModel;

#include <vector>
#include <string>

#include "../toolbox/vector.hpp"
#include "../toolbox/matrix.hpp"

class Entity
{
public:
    Vector3f position;
    float rotX, rotY, rotZ, rotRoll;
    float scale;
    bool visible;
    Vector3f baseColor;
    float baseAlpha = 1.0f;
    Matrix4f transformationMatrix;
    //render order is normally set in each TexturedModel, but can be overrided by using this
    char renderOrderOverride = 69;

    static void deleteModels(std::vector<TexturedModel*>* modelsToDelete);
    static void deleteCollisionModel(CollisionModel** colModelToDelete);
    //0 = rendered first (default), 1 = second, 2 = third, 3 = fourth + transparent (no depth testing)
    static void setModelsRenderOrder(std::vector<TexturedModel*>* models, char newOrder);

public:
    Entity();
    Entity(Vector3f* initialPosition, float rotX, float rotY, float rotZ, float scale);
    virtual ~Entity();

    virtual void step();

    virtual bool isVehicle() const;

    virtual bool isEmeraldPiece() const;

    virtual bool isGoalTrigger() const;

    virtual bool isEnemy() const;

    virtual bool isPoint() const;

    void increasePosition(float dx, float dy, float dz);

    void increaseRotation(float dx, float dy, float dz);

    virtual std::vector<TexturedModel*>* getModels();

    Vector3f* getPosition();
    void setPosition(Vector3f* newPosition);
    void setPosition(float newX, float newY, float newZ);

    Vector3f* getBaseColor();
    void setBaseColor(float red, float green, float blue);

    void updateTransformationMatrix();

    //for kart stages
    void updateTransformationMatrixYXZY();

    void updateTransformationMatrix(float scaleX, float scaleY, float scaleZ);

    void updateTransformationMatrixYXZ();

    virtual bool canLightdashOn() const;

    virtual bool canHomingAttackOn() const;

    virtual Vector3f getHomingCenter() const;

    virtual void die();
};
#endif
