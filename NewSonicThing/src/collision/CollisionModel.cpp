#include <cmath>
#include <list>

#include "triangle3d.h"
#include "../toolbox/vector.h"
#include "collisionmodel.h"
#include "../engineTester/main.h"
#include "quadtreenode.h"
#include "../toolbox/maths.h"


CollisionModel::CollisionModel()
{
	playerIsOn = false;
	quadTreeRoot = nullptr;
	treeMaxDepth = -1;
}

void CollisionModel::generateMinMaxValues()
{
	if (triangles.size() == 0)
	{
		return;
	}

	minX = triangles.front()->minX;
	maxX = triangles.front()->maxX;
	minY = triangles.front()->minY;
	maxY = triangles.front()->maxY;
	minZ = triangles.front()->minZ;
	maxZ = triangles.front()->maxZ;

	for (Triangle3D* tri : triangles)
	{
		minX = fmin(minX, tri->minX);
		maxX = fmax(maxX, tri->maxX);
		minY = fmin(minY, tri->minY);
		maxY = fmax(maxY, tri->maxY);
		minZ = fmin(minZ, tri->minZ);
		maxZ = fmax(maxZ, tri->maxZ);
	}
}

bool CollisionModel::hasQuadTree()
{
	return (quadTreeRoot != nullptr);
}

void CollisionModel::generateQuadTree(int maxDepth)
{
    if (quadTreeRoot != nullptr)
    {
        std::fprintf(stdout, "Warning: generating a quad tree on a collision model when it already has one. Memory leak!\n");
    }

	treeMaxDepth = maxDepth;
	const float pad = 200.0f; //padding to add to the edges of the quad tree
	quadTreeRoot = new QuadTreeNode(minX-pad, maxX+pad, minZ-pad, maxZ+pad, triangles, 0, maxDepth); INCR_NEW("QuadTreeNode");

    leafNodeWidth  = ((maxX+pad) - (minX-pad))/(1<<maxDepth);
    leafNodeHeight = ((maxZ+pad) - (minZ-pad))/(1<<maxDepth);
}

void CollisionModel::offsetModel(Vector3f* offset)
{
	float x = offset->x;
	float y = offset->y;
	float z = offset->z;

	for (Triangle3D* tri : triangles)
	{
		tri->p1X += x;
		tri->p2X += x;
		tri->p3X += x;
		tri->p1Y += y;
		tri->p2Y += y;
		tri->p3Y += y;
		tri->p1Z += z;
		tri->p2Z += z;
		tri->p3Z += z;

		tri->generateValues();
	}

	generateMinMaxValues();
}

void CollisionModel::rotateModelY(float yRot, Vector3f* center)
{
	float angleRad = Maths::toRadians(yRot);
	float cosAng = cosf(angleRad);
	float sinAng = sinf(angleRad);
	for (Triangle3D* tri : triangles)
	{
		float xDiff = tri->p1X - center->x;
		float zDiff = tri->p1Z - center->z;
		tri->p1X = center->x + ((xDiff)*cosAng - (zDiff)*sinAng);
		tri->p1Z = center->z - (-(zDiff)*cosAng - (xDiff)*sinAng);

		xDiff = tri->p2X - center->x;
		zDiff = tri->p2Z - center->z;
		tri->p2X = center->x + ((xDiff)*cosAng - (zDiff)*sinAng);
		tri->p2Z = center->z - (-(zDiff)*cosAng - (xDiff)*sinAng);

		xDiff = tri->p3X - center->x;
		zDiff = tri->p3Z - center->z;
		tri->p3X = center->x + ((xDiff)*cosAng - (zDiff)*sinAng);
		tri->p3Z = center->z - (-(zDiff)*cosAng - (xDiff)*sinAng);

		tri->generateValues();
	}

	generateMinMaxValues();
}

//makes a collision model be the transformed version of this collision model
void CollisionModel::transformModel(CollisionModel* targetModel, Vector3f* translate, float yRot, float zRot)
{
    if (targetModel->triangles.size() != triangles.size())
    {
        std::fprintf(stdout, "Warning: Trying to transform a collision model based on another collision model that doesn't have the same amount of triangles.\n");
        return;
    }

	float offX = translate->x;
	float offY = translate->y;
	float offZ = translate->z;

	float angleRad = Maths::toRadians(yRot);
	float cosAng = cosf(angleRad);
	float sinAng = sinf(angleRad);

	float angleRadZ = Maths::toRadians(zRot);
	float cosAngZ = cosf(angleRadZ);
	float sinAngZ = sinf(angleRadZ);

	//targetModel->deleteMe();
    int i = 0;
	for (Triangle3D* tri : triangles)
	{
		float xDiff = tri->p1X;
		float zDiff = tri->p1Z;
		float yDiff = tri->p1Y;

		float newX = (xDiff*cosAngZ - yDiff*sinAngZ);
		float newY = (yDiff*cosAngZ + xDiff*sinAngZ);
		Vector3f newP1(offX + (newX*cosAng - zDiff*sinAng), offY + newY, offZ + (zDiff*cosAng + newX*sinAng));

		xDiff = tri->p2X;
		zDiff = tri->p2Z;
		yDiff = tri->p2Y;
		newX = (xDiff*cosAngZ - yDiff*sinAngZ);
		newY = (yDiff*cosAngZ + xDiff*sinAngZ);
		Vector3f newP2(offX + (newX*cosAng - zDiff*sinAng), offY + newY, offZ + (zDiff*cosAng + newX*sinAng));

		xDiff = tri->p3X;
		zDiff = tri->p3Z;
		yDiff = tri->p3Y;
		newX = (xDiff*cosAngZ - yDiff*sinAngZ);
		newY = (yDiff*cosAngZ + xDiff*sinAngZ);
		Vector3f newP3(offX + (newX*cosAng - zDiff*sinAng), offY + newY, offZ + (zDiff*cosAng + newX*sinAng));

        //old, allocate on heap again
		//Triangle3D* newTri = new Triangle3D(&newP1, &newP2, &newP3, tri->type, tri->sound, tri->particle); INCR_NEW("Triangle3D");
		//targetModel->triangles.push_back(newTri);

        //new, keep already allocated triangles and change their values
        Triangle3D* newTri = targetModel->triangles[i];
        newTri->p1X      = newP1.x;
        newTri->p1Y      = newP1.y;
        newTri->p1Z      = newP1.z;
        newTri->p2X      = newP2.x;
        newTri->p2Y      = newP2.y;
        newTri->p2Z      = newP2.z;
        newTri->p3X      = newP3.x;
        newTri->p3Y      = newP3.y;
        newTri->p3Z      = newP3.z;
        newTri->type     = tri->type;
        newTri->sound    = tri->sound;
        newTri->particle = tri->particle;
        newTri->generateValues();
        i++;
	}

	targetModel->generateMinMaxValues();
}

void CollisionModel::transformModel(CollisionModel* /*targetModel*/, Vector3f* /*translate*/, float /*xRot*/, float /*yRot*/, float /*zRot*/)
{
	std::fprintf(stdout, "nothing\n");
}

//makes a collision model be the transformed version of this collision model
void CollisionModel::transformModel(CollisionModel* targetModel, Vector3f* translate, float yRot)
{
    if (targetModel->triangles.size() != triangles.size())
    {
        std::fprintf(stdout, "Warning: Trying to transform a collision model based on another collision model that doesn't have the same amount of triangles.\n");
        return;
    }

	float offX = translate->x;
	float offY = translate->y;
	float offZ = translate->z;

	float angleRad = Maths::toRadians(yRot);
	float cosAng = cosf(angleRad);
	float sinAng = sinf(angleRad);

	//targetModel->deleteMe();
    int i = 0;
	for (Triangle3D* tri : triangles)
	{
		float xDiff = tri->p1X;
		float zDiff = tri->p1Z;
		Vector3f newP1(offX + (xDiff*cosAng - zDiff*sinAng), offY + tri->p1Y, offZ + (zDiff*cosAng + xDiff*sinAng));

		xDiff = tri->p2X;
		zDiff = tri->p2Z;
		Vector3f newP2(offX + (xDiff*cosAng - zDiff*sinAng), offY + tri->p2Y, offZ + (zDiff*cosAng + xDiff*sinAng));

		xDiff = tri->p3X;
		zDiff = tri->p3Z;
		Vector3f newP3(offX + (xDiff*cosAng - zDiff*sinAng), offY + tri->p3Y, offZ + (zDiff*cosAng + xDiff*sinAng));

		//Triangle3D* newTri = new Triangle3D(&newP1, &newP2, &newP3, tri->type, tri->sound, tri->particle); INCR_NEW("Triangle3D");

		//targetModel->triangles.push_back(newTri);

        Triangle3D* newTri = targetModel->triangles[i];
        newTri->p1X      = newP1.x;
        newTri->p1Y      = newP1.y;
        newTri->p1Z      = newP1.z;
        newTri->p2X      = newP2.x;
        newTri->p2Y      = newP2.y;
        newTri->p2Z      = newP2.z;
        newTri->p3X      = newP3.x;
        newTri->p3Y      = newP3.y;
        newTri->p3Z      = newP3.z;
        newTri->type     = tri->type;
        newTri->sound    = tri->sound;
        newTri->particle = tri->particle;
        newTri->generateValues();
        i++;
	}

	targetModel->generateMinMaxValues();
}

void CollisionModel::transformModelWithScale(CollisionModel* targetModel, Vector3f* translate, float yRot, float scale)
{
    if (targetModel->triangles.size() != triangles.size())
    {
        std::fprintf(stdout, "Warning: Trying to transform a collision model based on another collision model that doesn't have the same amount of triangles.\n");
        return;
    }

	float offX = translate->x;
	float offY = translate->y;
	float offZ = translate->z;

	float angleRad = Maths::toRadians(yRot);
	float cosAng = cosf(angleRad);
	float sinAng = sinf(angleRad);

	//targetModel->deleteMe();
    int i = 0;
	for (Triangle3D* tri : triangles)
	{
		float xDiff = tri->p1X*scale;
		float zDiff = tri->p1Z*scale;
		Vector3f newP1(offX + (xDiff*cosAng - zDiff*sinAng), offY + tri->p1Y*scale, offZ + (zDiff*cosAng + xDiff*sinAng));

		xDiff = tri->p2X*scale;
		zDiff = tri->p2Z*scale;
		Vector3f newP2(offX + (xDiff*cosAng - zDiff*sinAng), offY + tri->p2Y*scale, offZ + (zDiff*cosAng + xDiff*sinAng));

		xDiff = tri->p3X*scale;
		zDiff = tri->p3Z*scale;
		Vector3f newP3(offX + (xDiff*cosAng - zDiff*sinAng), offY + tri->p3Y*scale, offZ + (zDiff*cosAng + xDiff*sinAng));

		//Triangle3D* newTri = new Triangle3D(&newP1, &newP2, &newP3, tri->type, tri->sound, tri->particle); INCR_NEW("Triangle3D");

		//targetModel->triangles.push_back(newTri);

        Triangle3D* newTri = targetModel->triangles[i];
        newTri->p1X      = newP1.x;
        newTri->p1Y      = newP1.y;
        newTri->p1Z      = newP1.z;
        newTri->p2X      = newP2.x;
        newTri->p2Y      = newP2.y;
        newTri->p2Z      = newP2.z;
        newTri->p3X      = newP3.x;
        newTri->p3Y      = newP3.y;
        newTri->p3Z      = newP3.z;
        newTri->type     = tri->type;
        newTri->sound    = tri->sound;
        newTri->particle = tri->particle;
        newTri->generateValues();
        i++;
	}

	targetModel->generateMinMaxValues();
}

//makes a collision model be the transformed version of this collision model
void CollisionModel::transformModel(CollisionModel* targetModel, Vector3f* translate)
{
    if (targetModel->triangles.size() != triangles.size())
    {
        std::fprintf(stdout, "Warning: Trying to transform a collision model based on another collision model that doesn't have the same amount of triangles.\n");
        return;
    }

	float offX = translate->x;
	float offY = translate->y;
	float offZ = translate->z;

	//targetModel->deleteMe();
    int i = 0;
	for (Triangle3D* tri : triangles)
	{
		Vector3f newP1(offX + tri->p1X, offY + tri->p1Y, offZ + tri->p1Z);
		Vector3f newP2(offX + tri->p2X, offY + tri->p2Y, offZ + tri->p2Z);
		Vector3f newP3(offX + tri->p3X, offY + tri->p3Y, offZ + tri->p3Z);

		//Triangle3D* newTri = new Triangle3D(&newP1, &newP2, &newP3, tri->type, tri->sound, tri->particle); INCR_NEW("Triangle3D");

		//targetModel->triangles.push_back(newTri);

        Triangle3D* newTri = targetModel->triangles[i];
        newTri->p1X      = newP1.x;
        newTri->p1Y      = newP1.y;
        newTri->p1Z      = newP1.z;
        newTri->p2X      = newP2.x;
        newTri->p2Y      = newP2.y;
        newTri->p2Z      = newP2.z;
        newTri->p3X      = newP3.x;
        newTri->p3Y      = newP3.y;
        newTri->p3Z      = newP3.z;
        newTri->type     = tri->type;
        newTri->sound    = tri->sound;
        newTri->particle = tri->particle;
        newTri->generateValues();
        i++;
	}

	targetModel->generateMinMaxValues();
}

void CollisionModel::deleteMe()
{
	//Delete triangles 
	for (Triangle3D* tri : triangles)
	{
		delete tri; INCR_DEL("Triangle3D");
	}

	triangles.clear();

	//Delete the quad tree nodes
	if (quadTreeRoot != nullptr)
	{
		quadTreeRoot->deleteMe();
		delete quadTreeRoot; INCR_DEL("QuadTreeNode");
		quadTreeRoot = nullptr;
	}
}

CollisionModel* CollisionModel::duplicateMe()
{
    CollisionModel* copy = new CollisionModel; INCR_NEW("CollisionModel");

	copy->treeMaxDepth    = this->treeMaxDepth;
    copy->leafNodeWidth   = this->leafNodeWidth;
    copy->leafNodeHeight  = this->leafNodeHeight;
	copy->maxX            = this->maxX;
	copy->minX            = this->minX;
	copy->maxY            = this->maxY;
	copy->minY            = this->minY;
	copy->maxZ            = this->maxZ;
	copy->minZ            = this->minZ;

    for (Triangle3D* tri : triangles)
    {
        Vector3f v1(tri->p1X, tri->p1Y, tri->p1Z);
        Vector3f v2(tri->p2X, tri->p2Y, tri->p2Z);
        Vector3f v3(tri->p3X, tri->p3Y, tri->p3Z);
        Triangle3D* newTri = new Triangle3D(&v1, &v2, &v3, tri->type, tri->sound, tri->particle); INCR_NEW("Triangle3D");
        newTri->generateValues();
        copy->triangles.push_back(newTri);
    }

    return copy;
}
