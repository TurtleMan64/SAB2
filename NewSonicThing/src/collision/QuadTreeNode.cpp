#include <list>

#include "quadtreenode.h"
#include "../engineTester/main.h"
#include "triangle3d.h"

//int iwfviub = 1;

QuadTreeNode::QuadTreeNode(float xBoundMin, float xBoundMax, float zBoundMin, float zBoundMax, std::list<Triangle3D*> pool, int depth, int depthMax)
{
	topLeft = nullptr;
	topRight = nullptr;
	botLeft = nullptr;
	botRight = nullptr;

	this->depth = depth;

	//std::fprintf(stdout, "v %f %f 0\n", xBoundMin, zBoundMin);
	//std::fprintf(stdout, "v %f %f 0\n", xBoundMax, zBoundMin);
	//std::fprintf(stdout, "v %f %f 0\n", xBoundMin, zBoundMax);
	//std::fprintf(stdout, "v %f %f 0\n", xBoundMax, zBoundMax);
	//std::fprintf(stdout, "f %d// %d// %d\n", iwfviub,   iwfviub+1, iwfviub+2);
	//std::fprintf(stdout, "f %d// %d// %d\n", iwfviub+1, iwfviub+2, iwfviub+3);
	//iwfviub+=4;

	if (depth == depthMax) //We are a leaf node = dont create any more nodes, we claim all remaining triangles
	{
		//Claim our triangles
		for (Triangle3D* tri : pool)
		{
			tris.push_back(tri);
			xMin = fminf(xMin, tri->minX);
			xMax = fmaxf(xMax, tri->maxX);
			yMin = fminf(yMin, tri->minY);
			yMax = fmaxf(yMax, tri->maxY);
			zMin = fminf(zMin, tri->minZ);
			zMax = fmaxf(zMax, tri->maxZ);
		}

		//std::fprintf(stdout, "leaf node rectangle = x: %f to %f,\tz: %f to %f\n", xBoundMin, xBoundMax, zBoundMin, zBoundMax);
		//std::fprintf(stdout, "v %f %f 0\n", xBoundMin, zBoundMin);
		//std::fprintf(stdout, "v %f %f 0\n", xBoundMax, zBoundMin);
		//std::fprintf(stdout, "v %f %f 0\n", xBoundMin, zBoundMax);
		//std::fprintf(stdout, "v %f %f 0\n", xBoundMax, zBoundMax);
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
				xMin = fminf(xMin, tri->minX);
				xMax = fmaxf(xMax, tri->maxX);
				yMin = fminf(yMin, tri->minY);
				yMax = fmaxf(yMax, tri->maxY);
				zMin = fminf(zMin, tri->minZ);
				zMax = fmaxf(zMax, tri->maxZ);
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
			topLeft = new QuadTreeNode(xBoundMin, xMid, zMid, zBoundMax, topLeftPool, depth + 1, depthMax); INCR_NEW("QuadTreeNode");
		}

		if (topRightPool.size() != 0)
		{
			topRight = new QuadTreeNode(xMid, xBoundMax, zMid, zBoundMax, topRightPool, depth + 1, depthMax); INCR_NEW("QuadTreeNode");
		}

		if (botLeftPool.size() != 0)
		{
			botLeft = new QuadTreeNode(xBoundMin, xMid, zBoundMin, zMid, botLeftPool, depth + 1, depthMax); INCR_NEW("QuadTreeNode");
		}

		if (botRightPool.size() != 0)
		{
			botRight = new QuadTreeNode(xMid, xBoundMax, zBoundMin, zMid, botRightPool, depth + 1, depthMax); INCR_NEW("QuadTreeNode");
		}
	}
}

void QuadTreeNode::deleteMe()
{
	if (topLeft != nullptr)
	{
		topLeft->deleteMe();
		delete topLeft; INCR_DEL("QuadTreeNode");
		topLeft = nullptr;
	}

	if (topRight != nullptr)
	{
		topRight->deleteMe();
		delete topRight; INCR_DEL("QuadTreeNode");
		topRight = nullptr;
	}

	if (botLeft != nullptr)
	{
		botLeft->deleteMe();
		delete botLeft; INCR_DEL("QuadTreeNode");
		botLeft = nullptr;
	}

	if (botRight != nullptr)
	{
		botRight->deleteMe();
		delete botRight; INCR_DEL("QuadTreeNode");
		botRight = nullptr;
	}

	tris.clear();
}
