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
    float xMinHorizontal;
    float xMaxHorizontal;
    float yMinHorizontal;
    float yMaxHorizontal;
    float zMinHorizontal;
    float zMaxHorizontal;
    std::vector<Triangle3D*> trisHorizontal;

    float xMinVertical;
    float xMaxVertical;
    float yMinVertical;
    float yMaxVertical;
    float zMinVertical;
    float zMaxVertical;
    std::vector<Triangle3D*> trisVertical;

    int depth; //this can probably be a char instead

    //This is for when you need to load in quad tree data manually
    QuadTreeNode();

    //This is for when you are having the quadtree do the work automatically
    QuadTreeNode(float xBoundMin, float xBoundMax, float zBoundMin, float zBoundMax, std::vector<Triangle3D*> pool, int depth, int depthMax);

    //Deletes all children nodes and clears triangle list
    void deleteMe();
};

#endif
