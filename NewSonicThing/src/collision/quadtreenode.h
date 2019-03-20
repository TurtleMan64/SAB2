#ifndef QUADTREENODE_H
#define QUADTREENODE_H

class Triangle3D;

#include <list>


class QuadTreeNode
{
public:
	QuadTreeNode* topLeft;
	QuadTreeNode* topRight;
	QuadTreeNode* botLeft;
	QuadTreeNode* botRight;

	float xMid;
	float zMid;

	//used for bounds checking. NOT the total size of the entire node, JUST of the node's triangles.
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;
	std::list<Triangle3D*> tris;

	int depth; //this can probably be a char instead

	QuadTreeNode(float xBoundMin, float xBoundMax, float zBoundMin, float zBoundMax, std::list<Triangle3D*> pool, int depth, int depthMax);

	//Deletes all children nodes and clears triangle list
	void deleteMe();
};

#endif