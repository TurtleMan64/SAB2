#ifndef COLLISIONCHECKER_H
#define COLLISIONCHECKER_H

class Triangle3D;
class CollisionModel;
class QuadTreeNode;

#include <math.h>
#include <list>
#include <unordered_set>
#include "../toolbox/vector.hpp"

class CollisionChecker
{
private:
    static Vector3f collidePosition;
    static Triangle3D* collideTriangle;
    static std::list<CollisionModel*> collideModels;
    static bool checkPlayer;
    static bool checkCamera;

    //given two coords, populates a set with all of the quadtree nodes
    // that the line can intersect with
    static void fetchNodesToCheck(
        std::unordered_set<QuadTreeNode*>* set, 
        CollisionModel* cm, 
        float x1, float z1, float x2, float z2);

    static int getBiggest(float A, float B, float C);

public:
    //static bool debug;
    //static std::string debugFilename;

public:
    static void initChecker();

    // Makes the next collision check set which collision
    // model the player has collided with, and sets that model
    // to touching the player.
    static void setCheckPlayer(bool checkPlayer);

    static void setCheckCamera(bool checkCamera);

    // Sets all collision models to not have the player on them
    static void falseAlarm();

    static bool checkCollision(
        float px1, float py1, float pz1,
        float px2, float py2, float pz2);

    static bool checkCollision(
        Vector3f* p1, Vector3f* p2);

    static bool checkPointInTriangle3D(
        float checkx, float checky, float checkz,
        Triangle3D* tri);

    static float pointToSegmentDistanceSquared(
        float x, float y,
        float x1, float y1,
        float x2, float y2);

    static bool checkPointInTriangle2D(
        float x,  float y,
        float x1, float y1,
        float x2, float y2,
        float x3, float y3);

    //delete's all collide models
    static void deleteAllCollideModels();

    //use this when reloading the same level - then you dont have to regenerate the quad trees
    static void deleteAllCollideModelsExceptQuadTrees();

    static void deleteCollideModel(CollisionModel* cm);

    static void addCollideModel(CollisionModel* cm);

    //based off of the last collision check
    static Triangle3D* getCollideTriangle();

    //based off of the last collision check
    static Vector3f* getCollidePosition();
};

#endif
