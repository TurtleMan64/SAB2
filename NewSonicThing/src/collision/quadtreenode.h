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

	std::list<Triangle3D*> tris;

	int depth; //this can probably be a char instead

	QuadTreeNode(float xBoundMin, float xBoundMax, float zBoundMin, float zBoundMax, std::list<Triangle3D*> pool, int depth, int depthMax);

	//Deletes all children nodes and clears triangle list
	void deleteMe();
};

#endif