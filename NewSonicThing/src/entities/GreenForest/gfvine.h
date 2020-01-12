#ifndef GF_VINE
#define GF_VINE

#include <list>
#include "../entity.h"

class TexturedModel;

class GF_Vine : public Entity
{
private:
    static std::list<TexturedModel*> models;

    Vector3f initialDirection; //so you can reset after vine is used
    Vector3f launchDirection; //once you finish, what direction do you go
    float launchSpeed;
    float rotateSpeed;
    Vector3f rotationAxis;
    float length; //length of the vine
    float numRotationsUntilFinish; //how many times does the vine swing around before releasing sonic?
    float inputLockTime;

    Vector3f currentDirection;
    float currentRotations;
    float angleChangeSpeed; //how fast does sonic get rotated around the vine
    bool playerIsHoldingOn;

    //used to do fast cheap checking before real check
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;

    Vector3f calcForwardDirection();

public:
    GF_Vine();

    GF_Vine(
        float x, float y, float z,
        float endX, float endY, float endZ,
        float rotAxisX, float rotAxisY, float rotAxisZ,
        float numRotations,
        float launchX, float launchY, float launchZ,
        float rotSpd,
        float launchSpeed,
        float inputLock);

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
