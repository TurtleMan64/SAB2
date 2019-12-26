#include <cmath>
#include <list>
#include <algorithm>
#include <unordered_set>

#include "collisionchecker.h"
#include "collisionmodel.h"
#include "triangle3d.h"
#include "quadtreenode.h"
#include "../engineTester/main.h"
#include "../toolbox/maths.h"

Vector3f CollisionChecker::collidePosition;
Triangle3D* CollisionChecker::collideTriangle;
std::list<CollisionModel*> CollisionChecker::collideModels;
bool CollisionChecker::checkPlayer;
bool CollisionChecker::debug;
//int debugCount = 1;

void CollisionChecker::initChecker()
{
    CollisionChecker::collidePosition.x = 0;
    CollisionChecker::collidePosition.y = 0;
    CollisionChecker::collidePosition.z = 0;

    CollisionChecker::collideTriangle = nullptr;

    CollisionChecker::checkPlayer = false;
}

void CollisionChecker::setCheckPlayer(bool checkPlayerNew)
{
    CollisionChecker::checkPlayer = checkPlayerNew;
}


void CollisionChecker::falseAlarm()
{
    for (CollisionModel* cm : CollisionChecker::collideModels)
    {
        cm->playerIsOn = false;
    }
}

bool CollisionChecker::checkCollision(Vector3f* p1, Vector3f* p2)
{
    return CollisionChecker::checkCollision(p1->x, p1->y, p1->z,
                                            p2->x, p2->y, p2->z);
}

int CollisionChecker::getBiggest(float A, float B, float C)
{
    A = fabsf(A);
    B = fabsf(B);
    C = fabsf(C);

    if (A > B)
    {
        if (A > C)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if (B > C)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}

bool CollisionChecker::checkCollision(
    float px1, float py1, float pz1,
    float px2, float py2, float pz2)
{
    bool triangleCollide = false;
    CollisionModel* finalModel = nullptr;

    //the distance squared from p1 to the current collision location
    float minDistSquared = 100000000000000.0f;

    //if (CollisionChecker::debug)
    {
        //std::fprintf(stdout, "vt 0 0\n");
        //std::fprintf(stdout, "vt 1 1\n");
    }

    for (CollisionModel* cm : CollisionChecker::collideModels)
    {
        if (CollisionChecker::checkPlayer)
        {
            cm->playerIsOn = false;
        }

        //Bounds check on entire model
        //if any of these are true, we can skip the model
        if (!((px1 < cm->minX && px2 < cm->minX) || (px1 > cm->maxX && px2 > cm->maxX) ||
              (pz1 < cm->minZ && pz2 < cm->minZ) || (pz1 > cm->maxZ && pz2 > cm->maxZ) ||
              (py1 < cm->minY && py2 < cm->minY) || (py1 > cm->maxY && py2 > cm->maxY)))
        {
            if (cm->hasQuadTree() == true)
            {
                std::unordered_set<QuadTreeNode*> set;
                CollisionChecker::fetchNodesToCheck(&set, cm, px1, pz1, px2, pz2);

                for (QuadTreeNode* node : set)
                {
                    //Bounds check on entire node
                    //if any of these are true, we can skip the node
                    if (!((px1 < node->xMin && px2 < node->xMin) || (px1 > node->xMax && px2 > node->xMax) ||
                          (pz1 < node->zMin && pz2 < node->zMin) || (pz1 > node->zMax && pz2 > node->zMax) ||
                          (py1 < node->yMin && py2 < node->yMin) || (py1 > node->yMax && py2 > node->yMax)))
                    {
                        for (Triangle3D* currTriangle : node->tris)
                        {
                            //if (CollisionChecker::debug)
                            {
                                //print the triangles being checked
                                //std::fprintf(stdout, "v %f %f %f\n", currTriangle->p1X, currTriangle->p1Y, currTriangle->p1Z);
                                //std::fprintf(stdout, "v %f %f %f\n", currTriangle->p2X, currTriangle->p2Y, currTriangle->p2Z);
                                //std::fprintf(stdout, "v %f %f %f\n", currTriangle->p3X, currTriangle->p3Y, currTriangle->p3Z);
                                //std::fprintf(stdout, "f %d// %d// %d\n", debugCount,   debugCount+1, debugCount+2);
                                //debugCount+=3;
                            }
                            
                            //Bounds check on individual triangle
                            //if any of these are true, we can skip the triangle
                            if (!((px1 < currTriangle->minX && px2 < currTriangle->minX) || (px1 > currTriangle->maxX && px2 > currTriangle->maxX) ||
                                  (pz1 < currTriangle->minZ && pz2 < currTriangle->minZ) || (pz1 > currTriangle->maxZ && pz2 > currTriangle->maxZ) ||
                                  (py1 < currTriangle->minY && py2 < currTriangle->minY) || (py1 > currTriangle->maxY && py2 > currTriangle->maxY)))
                            {
                                float A = currTriangle->A;
                                float B = currTriangle->B;
                                float C = currTriangle->C;
                                float D = currTriangle->D;

                                float numerator   = (A*px1 + B*py1 + C*pz1 + D);
                                float denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

                                if (denominator != 0)
                                {
                                    int idx = CollisionChecker::getBiggest(A, B, C);

                                    unsigned int firstAbove  = 0;
                                    unsigned int secondAbove = 0;

                                    switch (idx)
                                    {
                                        case 0: 
                                        {
                                            float planex1 = (((-B*py1) + (-C*pz1) - D)/A);
                                            float planex2 = (((-B*py2) + (-C*pz2) - D)/A);
                                            firstAbove  = Maths::getSignBit(px1 - planex1);
                                            secondAbove = Maths::getSignBit(px2 - planex2);
                                            break;
                                        }

                                        case 1: 
                                        {
                                            float planey1 = (((-A*px1) + (-C*pz1) - D)/B);
                                            float planey2 = (((-A*px2) + (-C*pz2) - D)/B);
                                            firstAbove  = Maths::getSignBit(py1 - planey1);
                                            secondAbove = Maths::getSignBit(py2 - planey2);
                                            break;
                                        }

                                        case 2: 
                                        {
                                            float planez1 = (((-B*py1) + (-A*px1) - D)/C);
                                            float planez2 = (((-B*py2) + (-A*px2) - D)/C);
                                            firstAbove  = Maths::getSignBit(pz1 - planez1);
                                            secondAbove = Maths::getSignBit(pz2 - planez2);
                                            break;
                                        }

                                        default:
                                            break;
                                    }

                                    if (secondAbove != firstAbove)
                                    {
                                        float u = (numerator/denominator);
                                        float cix = px1 + u*(px2 - px1);
                                        float ciy = py1 + u*(py2 - py1);
                                        float ciz = pz1 + u*(pz2 - pz1);

                                        if (checkPointInTriangle3D(cix, ciy, ciz, currTriangle))
                                        {
                                            //what is the distance to the triangle? set it to maxdist
                                            float thisDist = (cix - px1)*(cix - px1) + (ciy - py1)*(ciy - py1) + (ciz - pz1)*(ciz - pz1);
                                            if (thisDist < minDistSquared)
                                            {
                                                triangleCollide = true;
                                                collideTriangle = currTriangle;
                                                collidePosition.x = cix;
                                                collidePosition.y = ciy;
                                                collidePosition.z = ciz;
                                                minDistSquared = thisDist;
                                                finalModel = cm;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for (Triangle3D* currTriangle : cm->triangles)
                {
                    //Bounds check on individual triangle
                    //if any of these are true, we can skip the triangle
                    if (!((px1 < currTriangle->minX && px2 < currTriangle->minX) || (px1 > currTriangle->maxX && px2 > currTriangle->maxX) ||
                          (pz1 < currTriangle->minZ && pz2 < currTriangle->minZ) || (pz1 > currTriangle->maxZ && pz2 > currTriangle->maxZ) ||
                          (py1 < currTriangle->minY && py2 < currTriangle->minY) || (py1 > currTriangle->maxY && py2 > currTriangle->maxY)))
                    {
                        float A = currTriangle->A;
                        float B = currTriangle->B;
                        float C = currTriangle->C;
                        float D = currTriangle->D;

                        float numerator   = (A*px1 + B*py1 + C*pz1 + D);
                        float denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

                        if (denominator != 0)
                        {
                            int idx = CollisionChecker::getBiggest(A, B, C);

                            unsigned int firstAbove  = 0;
                            unsigned int secondAbove = 0;

                            switch (idx)
                            {
                                case 0: 
                                {
                                    float planex1 = (((-B*py1) + (-C*pz1) - D)/A);
                                    float planex2 = (((-B*py2) + (-C*pz2) - D)/A);
                                    firstAbove  = Maths::getSignBit(px1 - planex1);
                                    secondAbove = Maths::getSignBit(px2 - planex2);
                                    break;
                                }

                                case 1: 
                                {
                                    float planey1 = (((-A*px1) + (-C*pz1) - D)/B);
                                    float planey2 = (((-A*px2) + (-C*pz2) - D)/B);
                                    firstAbove  = Maths::getSignBit(py1 - planey1);
                                    secondAbove = Maths::getSignBit(py2 - planey2);
                                    break;
                                }

                                case 2: 
                                {
                                    float planez1 = (((-B*py1) + (-A*px1) - D)/C);
                                    float planez2 = (((-B*py2) + (-A*px2) - D)/C);
                                    firstAbove  = Maths::getSignBit(pz1 - planez1);
                                    secondAbove = Maths::getSignBit(pz2 - planez2);
                                    break;
                                }

                                default:
                                    break;
                            }

                            if (secondAbove != firstAbove)
                            {
                                float u = (numerator/denominator);
                                float cix = px1 + u*(px2 - px1);
                                float ciy = py1 + u*(py2 - py1);
                                float ciz = pz1 + u*(pz2 - pz1);

                                if (checkPointInTriangle3D(cix, ciy, ciz, currTriangle))
                                {
                                    //what is the distance to the triangle? set it to maxdist
                                    float thisDist = (cix - px1)*(cix - px1) + (ciy - py1)*(ciy - py1) + (ciz - pz1)*(ciz - pz1);
                                    if (thisDist < minDistSquared)
                                    {
                                        triangleCollide = true;
                                        collideTriangle = currTriangle;
                                        collidePosition.x = cix;
                                        collidePosition.y = ciy;
                                        collidePosition.z = ciz;
                                        minDistSquared = thisDist;
                                        finalModel = cm;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //if (CollisionChecker::debug && CollisionChecker::checkPlayer && finalModel != nullptr)
    {
        //print the path being checked
        //std::fprintf(stdout, "v %f %f %f\n", px1, py1, pz1);
        //std::fprintf(stdout, "v %f %f %f\n", px2, py2, pz2);
        //std::fprintf(stdout, "l %d/1 %d/1\n", debugCount, debugCount+1);
        //debugCount+=2;
    }
    //else if (CollisionChecker::debug && CollisionChecker::checkPlayer && finalModel == nullptr)
    {
        //print the path being checked
        //std::fprintf(stdout, "v %f %f %f\n", px1, py1, pz1);
        //std::fprintf(stdout, "v %f %f %f\n", px2, py2, pz2);
        //std::fprintf(stdout, "l %d/2 %d/2\n", debugCount, debugCount+1);
        //debugCount+=2;
    }

    if (CollisionChecker::checkPlayer && finalModel != nullptr)
    {
        finalModel->playerIsOn = true;
    }
    CollisionChecker::checkPlayer = false;
    CollisionChecker::debug = false;

    return triangleCollide;
}

bool CollisionChecker::checkPointInTriangle3D(
    float checkx, float checky, float checkz,
    Triangle3D* tri)
{
    float nX = fabsf(tri->normal.x);
    float nY = fabsf(tri->normal.y);
    float nZ = fabsf(tri->normal.z);

    if (nY > nX && nY > nZ)
    {
        //from the top
        return (checkPointInTriangle2D(
                checkx, checkz, 
                tri->p1X, tri->p1Z, 
                tri->p2X, tri->p2Z, 
                tri->p3X, tri->p3Z));
    }
    else if (nX > nZ)
    {
        //from the left
        return (checkPointInTriangle2D(
                checkz, checky, 
                tri->p1Z, tri->p1Y, 
                tri->p2Z, tri->p2Y, 
                tri->p3Z, tri->p3Y));
    }

    //from the front
    return (checkPointInTriangle2D(
            checkx, checky, 
            tri->p1X, tri->p1Y, 
            tri->p2X, tri->p2Y, 
            tri->p3X, tri->p3Y));
}

bool CollisionChecker::checkPointInTriangle2D(
    float x,  float y,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3)
{
    float denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
    float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denominator;
    float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denominator;
    float c = 1 - a - b;

    return (0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1);
}

//this is implemented in an "inneficient" way:
// check every node inside the rectangular
// box that contains the line (x1, z2) -> (x2, z2)
void CollisionChecker::fetchNodesToCheck(
        std::unordered_set<QuadTreeNode*>* set, 
        CollisionModel* cm, 
        float x1, float z1, float x2, float z2)
{
    QuadTreeNode* node = cm->quadTreeRoot;

    float leafWidth = cm->leafNodeWidth;
    float leafHeight = cm->leafNodeHeight;

    float cmXMid = cm->quadTreeRoot->xMid;
    float cmZMid = cm->quadTreeRoot->zMid;

    float xMin = fminf(x1, x2);
    float xMax = fmaxf(x1, x2);
    float zMin = fminf(z1, z2);
    float zMax = fmaxf(z1, z2);

    float xMinDiff = xMin - node->xMid;
    float xMaxDiff = xMax - node->xMid;
    float zMinDiff = zMin - node->zMid;
    float zMaxDiff = zMax - node->zMid;

    int xMinIdx = (int)(floorf(xMinDiff/leafWidth));
    int xMaxIdx = (int)(floorf(xMaxDiff/leafWidth));
    int zMinIdx = (int)(floorf(zMinDiff/leafHeight));
    int zMaxIdx = (int)(floorf(zMaxDiff/leafHeight));

    for (int x = xMinIdx; x <= xMaxIdx; x++)
    {
        for (int z = zMinIdx; z <= zMaxIdx; z++)
        {
            node = cm->quadTreeRoot;
            float testX = (0.5f+x)*leafWidth + cmXMid;
            float testZ = (0.5f+z)*leafHeight + cmZMid;

            while (node != nullptr)
            {
                set->insert(node);

                if (testX < node->xMid)
                {
                    if (testZ < node->zMid)
                    {
                        node = node->botLeft;
                    }
                    else
                    {
                        node = node->topLeft;
                    }
                }
                else
                {
                    if (testZ < node->zMid)
                    {
                        node = node->botRight;
                    }
                    else
                    {
                        node = node->topRight;
                    }
                }
            }
        }
    }
}

void CollisionChecker::deleteAllCollideModels()
{
    while (CollisionChecker::collideModels.size() > 0)
    {
        CollisionModel* cm = CollisionChecker::collideModels.front();
        CollisionChecker::collideModels.remove(cm);
        cm->deleteMe();
        delete cm; INCR_DEL("CollisionModel");
    }
}

void CollisionChecker::deleteAllCollideModelsExceptQuadTrees()
{
    std::list<CollisionModel*> modelsToDelete;

    for (CollisionModel* cm : CollisionChecker::collideModels)
    {
        if (cm->hasQuadTree() == false)
        {
            modelsToDelete.push_back(cm);
        }
    }

    for (CollisionModel* cm : modelsToDelete)
    {
        CollisionChecker::collideModels.remove(cm);
        cm->deleteMe();
        delete cm; INCR_DEL("CollisionModel");
    }
}

void CollisionChecker::deleteCollideModel(CollisionModel* cm)
{
    CollisionChecker::collideModels.remove(cm);
    cm->deleteMe();
    delete cm; INCR_DEL("CollisionModel");
}

//The model added must be created with the new keyword, as it will be deleted
// here later, via deleteCollideModel or deleteAlmostAllCollideModels call
void CollisionChecker::addCollideModel(CollisionModel* cm)
{
    CollisionChecker::collideModels.push_back(cm);
}

Triangle3D* CollisionChecker::getCollideTriangle()
{
    return CollisionChecker::collideTriangle;
}

Vector3f* CollisionChecker::getCollidePosition()
{
    return &CollisionChecker::collidePosition;
}
