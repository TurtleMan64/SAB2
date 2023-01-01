#ifndef ENTITIES_H
#define ENTITIES_H

class TexturedModel;
class CollisionModel;

#include <list>
#include "../toolbox/vector.hpp"
#include "../toolbox/matrix.hpp"
#include <string>


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

    static void deleteModels(std::list<TexturedModel*>* modelsToDelete);
    static void deleteCollisionModel(CollisionModel** colModelToDelete);
    //0 = rendered first (default), 1 = second, 2 = third, 3 = fourth + transparent (no depth testing)
    static void setModelsRenderOrder(std::list<TexturedModel*>* models, char newOrder);

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

    virtual std::list<TexturedModel*>* getModels();

    Vector3f* getPosition();
    void setPosition(Vector3f* newPosition);
    void setPosition(float newX, float newY, float newZ);

    Vector3f* getBaseColor();
    void setBaseColor(float red, float green, float blue);

    //float getRotX() const;
    //void setRotX(float newRotX);

    //float rotY const;
    //void rotY = (float newRotY);

    //float getRotZ() const;
    //void setRotZ(float newRotZ);

    //float getRotSpin() const;
    //void setRotSpin(float newRotSpin);

    //float getScale() const;
    //void setScale(float newScale);

    //bool getVisible() const;
    //void setVisible(bool newVisible);

    //float position.x const;

    //float position.y const;

    //float position.z const;

    //void position.x = (float newX);

    //void position.y = (float newY);

    //void position.z = (float newZ);

    //Matrix4f* getTransformationMatrix();

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
