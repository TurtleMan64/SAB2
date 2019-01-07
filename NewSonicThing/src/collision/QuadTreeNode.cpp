#include <list>

#include "quadtreenode.h"
#include "../engineTester/main.h"
#include "triangle3d.h"

QuadTreeNode::QuadTreeNode(float xBoundMin, float xBoundMax, float zBoundMin, float zBoundMax, std::list<Triangle3D*> pool, int depth, int depthMax)
{
	topLeft = nullptr;
	topRight = nullptr;
	botLeft = nullptr;
	botRight = nullptr;

	this->depth = depth;

	if (depth == depthMax) //We are a leaf node = dont create any more nodes, we claim all remaining triangles
	{
		//Claim our triangles
		for (Triangle3D* tri : pool)
		{
			tris.push_back(tri);
		}
	}
	else //claim triangles that cross the border, create child nodes if nessesary
	{
		xMid = (xBoundMin + xBoundMax) / 2;
		zMid = (zBoundMin + zBoundMax) / 2;

		std::list<Triangle3D*> topLeftPool;
		std::list<Triangle3D*> topRightPool;
		std::list<Triangle3D*> botLeftPool;
		std::list<Triangle3D*> botRightPool;

		//Claim our triangles, split up pool into 4 smaller pools
		for (Triangle3D* tri : pool)
		{
			if ((tri->minX <= xMid && tri->maxX >= xMid) ||
				(tri->minZ <= zMid && tri->maxZ >= zMid))
			{
				tris.push_back(tri); //We claim the triangle
			}
			else if (tri->maxX <= xMid)
			{
				if (tri->maxZ <= zMid)
				{
					botLeftPool.push_back(tri);
				}
				else
				{
					topLeftPool.push_back(tri);
				}
			}
			else
			{
				if (tri->maxZ <= zMid)
				{
					botRightPool.push_back(tri);
				}
				else
				{
					topRightPool.push_back(tri);
				}
			}
		}

		if (topLeftPool.size() != 0)
		{
			topLeft = new QuadTreeNode(xBoundMin, xMid, zMid, zBoundMax, topLeftPool, depth + 1, depthMax);
			INCR_NEW
		}

		if (topRightPool.size() != 0)
		{
			topRight = new QuadTreeNode(xMid, xBoundMax, zMid, zBoundMax, topRightPool, depth + 1, depthMax);
			INCR_NEW
		}

		if (botLeftPool.size() != 0)
		{
			botLeft = new QuadTreeNode(xBoundMin, xMid, zBoundMin, zMid, botLeftPool, depth + 1, depthMax);
			INCR_NEW
		}

		if (botRightPool.size() != 0)
		{
			botRight = new QuadTreeNode(xMid, xBoundMax, zBoundMin, zMid, botRightPool, depth + 1, depthMax);
			INCR_NEW
		}
	}
}

void QuadTreeNode::deleteMe()
{
	if (topLeft != nullptr)
	{
		topLeft->deleteMe();
		delete topLeft;
		INCR_DEL
		topLeft = nullptr;
	}

	if (topRight != nullptr)
	{
		topRight->deleteMe();
		delete topRight;
		INCR_DEL
		topRight = nullptr;
	}

	if (botLeft != nullptr)
	{
		botLeft->deleteMe();
		delete botLeft;
		INCR_DEL
		botLeft = nullptr;
	}

	if (botRight != nullptr)
	{
		botRight->deleteMe();
		delete botRight;
		INCR_DEL
		botRight = nullptr;
	}

	tris.clear();
}