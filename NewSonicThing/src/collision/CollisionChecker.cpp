#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <list>
#include <unordered_set>

#include "collisionchecker.hpp"
#include "collisionmodel.hpp"
#include "triangle3d.hpp"
#include "quadtreenode.hpp"
#include "../engineTester/main.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/vector.hpp"

Vector3f CollisionChecker::collidePosition(0, 0, 0);
Triangle3D* CollisionChecker::collideTriangle = nullptr;
std::list<CollisionModel*> CollisionChecker::collideModels;
bool CollisionChecker::checkPlayer = false;
bool CollisionChecker::checkCamera = false;

//bool CollisionChecker::debug = false;
//std::string CollisionChecker::debugFilename = "collision1.obj";
//int debugCount = 1;

//std::vector<std::string> out;

void CollisionChecker::initChecker()
{
    //out.reserve(1000000);
}

void CollisionChecker::setCheckPlayer(bool checkPlayerNew)
{
    CollisionChecker::checkPlayer = checkPlayerNew;
}

void CollisionChecker::setCheckCamera(bool checkCameraNew)
{
    CollisionChecker::checkCamera = checkCameraNew;
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
    A = std::abs(A);
    B = std::abs(B);
    C = std::abs(C);

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

//long long tCountTotal = 0;
//long long tCountN = 0;
//int tCountWorse = 0;

bool CollisionChecker::checkCollision(
    float px1, float py1, float pz1,
    float px2, float py2, float pz2)
{
    ANALYSIS_START("Collision Checking");
    bool triangleCollide = false;
    CollisionModel* finalModel = nullptr;

    //the distance squared from p1 to the current collision location
    float minDistSquared = 100000000000000.0f;
     
    //out.clear();
    //if (CollisionChecker::debug)
    //{
    //    out.push_back("vt 0 0");
    //    out.push_back("vn 0 1 0");
    //}

    for (CollisionModel* cm : CollisionChecker::collideModels)
    {
        if (CollisionChecker::checkPlayer)
        {
            cm->playerIsOn = false;
        }

        if (!cm->tangible)
        {
            continue;
        }

        //Bounds check on entire model
        //if any of these are true, we can skip the model
        if (!((px1 < cm->minX && px2 < cm->minX) || (px1 > cm->maxX && px2 > cm->maxX) ||
              (pz1 < cm->minZ && pz2 < cm->minZ) || (pz1 > cm->maxZ && pz2 > cm->maxZ) ||
              (py1 < cm->minY && py2 < cm->minY) || (py1 > cm->maxY && py2 > cm->maxY)))
        {
            if (cm->hasQuadTree())
            {
                std::unordered_set<QuadTreeNode*> set;
                CollisionChecker::fetchNodesToCheck(&set, cm, px1, pz1, px2, pz2);

                //int nodeCount = 0;
                //int nodeCount2 = 0;
                //int triangleCount = 0;
                //int deepCount = 0;
                //debugCount = 1;
                // tCountN++;
                //int nodeCount = 0;

                for (QuadTreeNode* node : set)
                {
                    //nodeCount++;
                    //Bounds check on entire node
                    //if any of these are true, we can skip the node
                    if (!((px1 < node->xMinHorizontal && px2 < node->xMinHorizontal) || (px1 > node->xMaxHorizontal && px2 > node->xMaxHorizontal) ||
                          (pz1 < node->zMinHorizontal && pz2 < node->zMinHorizontal) || (pz1 > node->zMaxHorizontal && pz2 > node->zMaxHorizontal) ||
                          (py1 < node->yMinHorizontal && py2 < node->yMinHorizontal) || (py1 > node->yMaxHorizontal && py2 > node->yMaxHorizontal)))
                    {
                        //if (CollisionChecker::debug)
                        //{
                        //    //std::fprintf(fptr, "g group_horiz_%d\n", nodeCount);
                        //    //std::fprintf(fptr, "o object_horiz_%d\n", nodeCount);
                        //    out.push_back("g group_horiz_" + std::to_string(nodeCount));
                        //    out.push_back("o object_horiz_" + std::to_string(nodeCount));
                        //    nodeCount++;
                        //}

                        //nodeCount2++;
                        for (Triangle3D* currTriangle : node->trisHorizontal)
                        {
                            //triangleCount++;
                            //tCountTotal++;
                            //if (CollisionChecker::debug)
                            //{
                            //    //print the triangles being checked
                            //    //std::fprintf(fptr, "v %f %f %f\n", currTriangle->p1X, currTriangle->p1Y, currTriangle->p1Z);
                            //    //std::fprintf(fptr, "v %f %f %f\n", currTriangle->p2X, currTriangle->p2Y, currTriangle->p2Z);
                            //    //std::fprintf(fptr, "v %f %f %f\n", currTriangle->p3X, currTriangle->p3Y, currTriangle->p3Z);
                            //    //std::fprintf(fptr, "f %d/1/1 %d/1/1 %d/1/1\n", debugCount, debugCount+1, debugCount+2);
                            //
                            //    out.push_back("v " + std::to_string(currTriangle->p1X) + " " + std::to_string(currTriangle->p1Y) + " " + std::to_string(currTriangle->p1Z));
                            //    out.push_back("v " + std::to_string(currTriangle->p2X) + " " + std::to_string(currTriangle->p2Y) + " " + std::to_string(currTriangle->p2Z));
                            //    out.push_back("v " + std::to_string(currTriangle->p3X) + " " + std::to_string(currTriangle->p3Y) + " " + std::to_string(currTriangle->p3Z));
                            //    out.push_back("f " + std::to_string(debugCount) + "/1/1 " + std::to_string(debugCount+1) + "/1/1 " + std::to_string(debugCount+2) + "/1/1");
                            //    debugCount+=3;
                            //}
                            
                            //Bounds check on individual triangle
                            //if any of these are true, we can skip the triangle
                            if (!((px1 < currTriangle->minX && px2 < currTriangle->minX) || (px1 > currTriangle->maxX && px2 > currTriangle->maxX) ||
                                  (pz1 < currTriangle->minZ && pz2 < currTriangle->minZ) || (pz1 > currTriangle->maxZ && pz2 > currTriangle->maxZ) ||
                                  (py1 < currTriangle->minY && py2 < currTriangle->minY) || (py1 > currTriangle->maxY && py2 > currTriangle->maxY)))
                            {
                                //deepCount++;
                                float A = currTriangle->A;
                                float B = currTriangle->B;
                                float C = currTriangle->C;
                                float D = currTriangle->D;

                                float numerator   = (A*px1 + B*py1 + C*pz1 + D);
                                float denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

                                if (denominator != 0)
                                {
                                    int idx = CollisionChecker::getBiggest(A, B, C);

                                    int firstAbove  = 0;
                                    int secondAbove = 0;

                                    switch (idx)
                                    {
                                        case 0: 
                                        {
                                            float planex1 = (((-B*py1) + (-C*pz1) - D)/A);
                                            float planex2 = (((-B*py2) + (-C*pz2) - D)/A);
                                            firstAbove  = Maths::sign(px1 - planex1);
                                            secondAbove = Maths::sign(px2 - planex2);
                                            break;
                                        }

                                        case 1: 
                                        {
                                            float planey1 = (((-A*px1) + (-C*pz1) - D)/B);
                                            float planey2 = (((-A*px2) + (-C*pz2) - D)/B);
                                            firstAbove  = Maths::sign(py1 - planey1);
                                            secondAbove = Maths::sign(py2 - planey2);
                                            break;
                                        }

                                        case 2: 
                                        {
                                            float planez1 = (((-B*py1) + (-A*px1) - D)/C);
                                            float planez2 = (((-B*py2) + (-A*px2) - D)/C);
                                            firstAbove  = Maths::sign(pz1 - planez1);
                                            secondAbove = Maths::sign(pz2 - planez2);
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
                                                if (!CollisionChecker::checkCamera || !currTriangle->isNoCam())
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

                    if (!((px1 < node->xMinVertical && px2 < node->xMinVertical) || (px1 > node->xMaxVertical && px2 > node->xMaxVertical) ||
                          (pz1 < node->zMinVertical && pz2 < node->zMinVertical) || (pz1 > node->zMaxVertical && pz2 > node->zMaxVertical) ||
                          (py1 < node->yMinVertical && py2 < node->yMinVertical) || (py1 > node->yMaxVertical && py2 > node->yMaxVertical)))
                    {
                        //nodeCount2++;

                        //if (CollisionChecker::debug)
                        //{
                        //    //std::fprintf(fptr, "g group_vert_%d\n", nodeCount);
                        //    //std::fprintf(fptr, "o object_vert_%d\n", nodeCount);
                        //
                        //    out.push_back("g group_horiz_" + std::to_string(nodeCount));
                        //    out.push_back("o object_horiz_" + std::to_string(nodeCount));
                        //    nodeCount++;
                        //}

                        for (Triangle3D* currTriangle : node->trisVertical)
                        {
                            //triangleCount++;
                            //tCountTotal++;
                            //if (CollisionChecker::debug)
                            //{
                            //    //print the triangles being checked
                            //    //std::fprintf(fptr, "v %f %f %f\n", currTriangle->p1X, currTriangle->p1Y, currTriangle->p1Z);
                            //    //std::fprintf(fptr, "v %f %f %f\n", currTriangle->p2X, currTriangle->p2Y, currTriangle->p2Z);
                            //    //std::fprintf(fptr, "v %f %f %f\n", currTriangle->p3X, currTriangle->p3Y, currTriangle->p3Z);
                            //    //std::fprintf(fptr, "f %d/1/1 %d/1/1 %d/1/1\n", debugCount, debugCount+1, debugCount+2);
                            //
                            //    out.push_back("v " + std::to_string(currTriangle->p1X) + " " + std::to_string(currTriangle->p1Y) + " " + std::to_string(currTriangle->p1Z));
                            //    out.push_back("v " + std::to_string(currTriangle->p2X) + " " + std::to_string(currTriangle->p2Y) + " " + std::to_string(currTriangle->p2Z));
                            //    out.push_back("v " + std::to_string(currTriangle->p3X) + " " + std::to_string(currTriangle->p3Y) + " " + std::to_string(currTriangle->p3Z));
                            //    out.push_back("f " + std::to_string(debugCount) + "/1/1 " + std::to_string(debugCount+1) + "/1/1 " + std::to_string(debugCount+2) + "/1/1");
                            //    debugCount+=3;
                            //}
                            
                            //Bounds check on individual triangle
                            //if any of these are true, we can skip the triangle
                            if (!((px1 < currTriangle->minX && px2 < currTriangle->minX) || (px1 > currTriangle->maxX && px2 > currTriangle->maxX) ||
                                  (pz1 < currTriangle->minZ && pz2 < currTriangle->minZ) || (pz1 > currTriangle->maxZ && pz2 > currTriangle->maxZ) ||
                                  (py1 < currTriangle->minY && py2 < currTriangle->minY) || (py1 > currTriangle->maxY && py2 > currTriangle->maxY)))
                            {
                                //deepCount++;
                                float A = currTriangle->A;
                                float B = currTriangle->B;
                                float C = currTriangle->C;
                                float D = currTriangle->D;

                                float numerator   = (A*px1 + B*py1 + C*pz1 + D);
                                float denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

                                if (denominator != 0)
                                {
                                    int idx = CollisionChecker::getBiggest(A, B, C);

                                    int firstAbove  = 0;
                                    int secondAbove = 0;

                                    switch (idx)
                                    {
                                        case 0: 
                                        {
                                            float planex1 = (((-B*py1) + (-C*pz1) - D)/A);
                                            float planex2 = (((-B*py2) + (-C*pz2) - D)/A);
                                            firstAbove  = Maths::sign(px1 - planex1);
                                            secondAbove = Maths::sign(px2 - planex2);
                                            break;
                                        }

                                        case 1: 
                                        {
                                            float planey1 = (((-A*px1) + (-C*pz1) - D)/B);
                                            float planey2 = (((-A*px2) + (-C*pz2) - D)/B);
                                            firstAbove  = Maths::sign(py1 - planey1);
                                            secondAbove = Maths::sign(py2 - planey2);
                                            break;
                                        }

                                        case 2: 
                                        {
                                            float planez1 = (((-B*py1) + (-A*px1) - D)/C);
                                            float planez2 = (((-B*py2) + (-A*px2) - D)/C);
                                            firstAbove  = Maths::sign(pz1 - planez1);
                                            secondAbove = Maths::sign(pz2 - planez2);
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
                                                if (!CollisionChecker::checkCamera || !currTriangle->isNoCam())
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
                //if (triangleCount > tCountWorse)
                {
                    //tCountWorse = triangleCount;
                }
                //printf("nodeCount     = %d\n", nodeCount);
                //printf("nodeCount2    = %d\n", nodeCount2);
                //printf("triangleCount = %d\n", triangleCount);
                //printf("deepCount     = %d\n\n", deepCount);
                //printf("avg   t count = %lld\n", tCountTotal/tCountN);
                //printf("worst t count = %d\n", tCountWorse);
                //printf("\n");
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

                            int firstAbove  = 0;
                            int secondAbove = 0;

                            switch (idx)
                            {
                                case 0: 
                                {
                                    float planex1 = (((-B*py1) + (-C*pz1) - D)/A);
                                    float planex2 = (((-B*py2) + (-C*pz2) - D)/A);
                                    firstAbove  = Maths::sign(px1 - planex1);
                                    secondAbove = Maths::sign(px2 - planex2);
                                    break;
                                }

                                case 1: 
                                {
                                    float planey1 = (((-A*px1) + (-C*pz1) - D)/B);
                                    float planey2 = (((-A*px2) + (-C*pz2) - D)/B);
                                    firstAbove  = Maths::sign(py1 - planey1);
                                    secondAbove = Maths::sign(py2 - planey2);
                                    break;
                                }

                                case 2: 
                                {
                                    float planez1 = (((-B*py1) + (-A*px1) - D)/C);
                                    float planez2 = (((-B*py2) + (-A*px2) - D)/C);
                                    firstAbove  = Maths::sign(pz1 - planez1);
                                    secondAbove = Maths::sign(pz2 - planez2);
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
                                        if (!CollisionChecker::checkCamera || !currTriangle->isNoCam())
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
    }

    //if (CollisionChecker::debug && CollisionChecker::checkPlayer && finalModel != nullptr)
    {
        //print the path being checked
        //printf("v %f %f %f\n", px1, py1, pz1);
        //printf("v %f %f %f\n", px2, py2, pz2);
        //printf("l %d/1 %d/1\n", debugCount, debugCount+1);
        //debugCount+=2;
    }
    //else if (CollisionChecker::debug && CollisionChecker::checkPlayer && finalModel == nullptr)
    {
        //print the path being checked
        //printf("v %f %f %f\n", px1, py1, pz1);
        //printf("v %f %f %f\n", px2, py2, pz2);
        //printf("l %d/2 %d/2\n", debugCount, debugCount+1);
        //debugCount+=2;
    }

    //if (CollisionChecker::debug)
    //{
    //    fclose(fptr);
    //}

    //if (CollisionChecker::debug && CollisionChecker::checkPlayer && finalModel == nullptr)
    //{
    //    out.push_back("v " + std::to_string(px1) + " " + std::to_string(py1) + " " + std::to_string(pz1));
    //    out.push_back("v " + std::to_string(px2) + " " + std::to_string(py2) + " " + std::to_string(pz2));
    //    out.push_back("l " + std::to_string(debugCount) + "/2 " + std::to_string(debugCount+1) + "/2");
    //
    //    FILE* fptr = fopen(CollisionChecker::debugFilename.c_str(), "w");
    //
    //    for (int i = 0; i < out.size(); i++)
    //    {
    //        std::fprintf(fptr, "%s\n", out[i].c_str());
    //    }
    //
    //    fclose(fptr);
    //
    //    //CollisionChecker::debug = false;
    //}

    if (CollisionChecker::checkPlayer && finalModel != nullptr)
    {
        finalModel->playerIsOn = true;
    }
    CollisionChecker::checkPlayer = false;
    CollisionChecker::checkCamera = false;
    //CollisionChecker::debug = false;

    ANALYSIS_DONE("Collision Checking");
    return triangleCollide;
}

bool CollisionChecker::checkPointInTriangle3D(
    float checkx, float checky, float checkz,
    Triangle3D* tri)
{
    float nX = std::abs(tri->normal.x);
    float nY = std::abs(tri->normal.y);
    float nZ = std::abs(tri->normal.z);

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

//https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
float CollisionChecker::pointToSegmentDistanceSquared(
    float x, float y,
    float x1, float y1,
    float x2, float y2)
{
    Vector2f v(x1, y1);
    Vector2f w(x2, y2);
    Vector2f p(x, y);

    float l2 = (w - v).lengthSquared();
    if (l2 == 0.0f) //line segment is same points
    {
        return (w - p).lengthSquared();
    }

    Vector2f wv = w - v;
    float t =  ((p - v).dot(&wv)) / l2;

    if (t > 1.0f)
    {
        t = 1.0f;
    }
    else if (t < 0.0f)
    {
        t = 0.0f;
    }

    Vector2f projection = v + (w - v).scaleCopy(t);

    return (p - projection).lengthSquared();
}

//http://totologic.blogspot.com/2014/01/accurate-point-in-triangle-test.html
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

    if (0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1)
    {
        return true;
    }

    //handle a literal edge case where the point falls extremely close to an edge
    const float EPSILON_SQUARED = 0.0001f*0.0001f;
    float distToEdgeSquared = 0.0f;

    distToEdgeSquared = pointToSegmentDistanceSquared(x, y, x1, y1, x2, y2);
    if (distToEdgeSquared <= EPSILON_SQUARED)
    {
        printf("Edge case detected with distToEdgeSquared %f\n", distToEdgeSquared);
        return true;
    }

    distToEdgeSquared = pointToSegmentDistanceSquared(x, y, x2, y2, x3, y3);
    if (distToEdgeSquared <= EPSILON_SQUARED)
    {
        printf("Edge case detected with distToEdgeSquared %f\n", distToEdgeSquared);
        return true;
    }

    distToEdgeSquared = pointToSegmentDistanceSquared(x, y, x3, y3, x1, y1);
    if (distToEdgeSquared <= EPSILON_SQUARED)
    {
        printf("Edge case detected with distToEdgeSquared %f\n", distToEdgeSquared);
        return true;
    }
    
    return false;
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
