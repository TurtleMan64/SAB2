#ifndef COLLISIONMODEL_H
#define COLLISIONMODEL_H

class Vector3f;
class Triangle3D;
class QuadTreeNode;

#include <math.h>
#include <list>


class CollisionModel
{
public:
	std::list<Triangle3D*> triangles;

	bool playerIsOn;

	QuadTreeNode* quadTreeRoot;
	int treeMaxDepth;

	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;

	CollisionModel();

	void generateMinMaxValues();

	bool hasQuadTree();

	void generateQuadTree(int maxDepth);

	void offsetModel(Vector3f* offset);

	void rotateModelY(float yRot, Vector3f* center);

	//makes a collision model be the transformed version of this collision model
	void transformModel(CollisionModel* targetModel, Vector3f* translate, float yRot, float zRot);

	//rotation order: x, z, y
	void transformModel(CollisionModel* targetModel, Vector3f* translate, float xRot, float yRot, float zRot);

	//makes a collision model be the transformed version of this collision model
	void transformModel(CollisionModel* targetModel, Vector3f* translate, float yRot);

	//makes a collision model be the transformed version of this collision model
	void transformModelWithScale(CollisionModel* targetModel, Vector3f* translate, float yRot, float scale);

	//makes a collision model be the transformed version of this collision model
	void transformModel(CollisionModel* targetModel, Vector3f* translate);

	//calls delete on every Triangle3D contained within triangles list, 
	// and every QuadTreeNode in this quad tree. 
	// this MUST be called before this object is deleted, or you memory leak
	// the triangles in the list and the nodes!
	void deleteMe();
};

#endif