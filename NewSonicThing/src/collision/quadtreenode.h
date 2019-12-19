#ifndef QUADTREENODE_H
#define QUADTREENODE_H

class Triangle3D;

#include <vector>


class QuadTreeNode
{
public:
    QuadTreeNode* topLeft  = nullptr;
    QuadTreeNode* topRight = nullptr;
    QuadTreeNode* botLeft  = nullptr;
    QuadTreeNode* botRight = nullptr;

    float xMid;
    float zMid;

    //used for bounds checking. NOT the total size of the entire node, JUST of the node's triangles.
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;
    std::vector<Triangle3D*> tris;

    int depth; //this can probably be a char instead

    //This is for when you need to load in quad tree data manually
    QuadTreeNode();

    //This is for when you are having the quadtree do the work automatically
    QuadTreeNode(float xBoundMin, float xBoundMax, float zBoundMin, float zBoundMax, std::vector<Triangle3D*> pool, int depth, int depthMax);

    //Deletes all children nodes and clears triangle list
    void deleteMe();
};

#endif
