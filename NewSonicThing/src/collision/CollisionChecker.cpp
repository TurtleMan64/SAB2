#include <cmath>
#include <list>
#include <algorithm>


#include "collisionchecker.h"
#include "collisionmodel.h"
#include "triangle3d.h"
#include "quadtreenode.h"
#include "../engineTester/main.h"

Vector3f CollisionChecker::collidePosition;
Triangle3D* CollisionChecker::collideTriangle;
//CollisionModel* CollisionChecker::stageCollideModel;
std::list<CollisionModel*> CollisionChecker::collideModels;
bool CollisionChecker::checkPlayer;



void CollisionChecker::initChecker()
{
	CollisionChecker::collidePosition.x = 0;
	CollisionChecker::collidePosition.y = 0;
	CollisionChecker::collidePosition.z = 0;

	CollisionChecker::collideTriangle = nullptr;

	CollisionChecker::checkPlayer = false;
}

void CollisionChecker::setCheckPlayer()
{
	CollisionChecker::checkPlayer = true;
}


void CollisionChecker::falseAlarm()
{
	for (CollisionModel* cm : CollisionChecker::collideModels)
	{
		cm->playerIsOn = false;
	}
}

bool CollisionChecker::checkCollision(
	float px1, float py1, float pz1,
	float px2, float py2, float pz2)
{
	bool triangleCollide = false;
	CollisionModel* finalModel = nullptr;
	float firstAbove = -2;
	float secondAbove = -2;
	float tx1 = 0, tx2 = 0, tx3 = 0, ty1 = 0, ty2 = 0, ty3 = 0, tz1 = 0, tz2 = 0, tz3 = 0;
	float A, B, C, D;
	float numerator, denominator, u;

	float minDist = -1;

	float checkRadius = 8 + sqrtf(powf(px2 - px1, 2) + powf(py2 - py1, 2) + powf(pz2 - pz1, 2));


	//CollisionChecker::collideModels.push_back(CollisionChecker::stageCollideModel);
	for (CollisionModel* cm : CollisionChecker::collideModels)
	{
		if (CollisionChecker::checkPlayer)
		{
			cm->playerIsOn = false;
		}

		//Bounds check on entire model
		if (px1 - checkRadius <= cm->maxX && px1 + checkRadius >= cm->minX &&
			pz1 - checkRadius <= cm->maxZ && pz1 + checkRadius >= cm->minZ &&
			py1 - checkRadius <= cm->maxY && py1 + checkRadius >= cm->minY)
		{
			if (cm->hasQuadTree() == true)
			{
				QuadTreeNode* node = cm->quadTreeRoot;

				std::list<QuadTreeNode*> alreadySearchedNodes;

				int treeMaxDepth = cm->treeMaxDepth;

				//search for (px1, pz1)
				while (node != nullptr)
				{
					if (std::find(alreadySearchedNodes.begin(), alreadySearchedNodes.end(), node) == alreadySearchedNodes.end())
					{
						alreadySearchedNodes.push_back(node);

						//check through node.tris;
						for (Triangle3D* currTriangle : node->tris)
						{
							//Bounds check on individual triangle
							if (px1 - checkRadius <= currTriangle->maxX && px1 + checkRadius >= currTriangle->minX &&
								pz1 - checkRadius <= currTriangle->maxZ && pz1 + checkRadius >= currTriangle->minZ &&
								py1 - checkRadius <= currTriangle->maxY && py1 + checkRadius >= currTriangle->minY)
							{
								tx1 = currTriangle->p1X;
								tx2 = currTriangle->p2X;
								tx3 = currTriangle->p3X;
								ty1 = currTriangle->p1Y;
								ty2 = currTriangle->p2Y;
								ty3 = currTriangle->p3Y;
								tz1 = currTriangle->p1Z;
								tz2 = currTriangle->p2Z;
								tz3 = currTriangle->p3Z;
								A = currTriangle->A;
								B = currTriangle->B;
								C = currTriangle->C;
								D = currTriangle->D;

								numerator = (A*px1 + B*py1 + C*pz1 + D);
								denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

								if (denominator != 0)
								{
									u = (numerator / denominator);
									float cix = px1 + u*(px2 - px1);
									float ciy = py1 + u*(py2 - py1);
									float ciz = pz1 + u*(pz2 - pz1);

									if (B != 0)
									{
										float planey1 = (((-A*px1) + (-C*pz1) - D) / B);
										float planey2 = (((-A*px2) + (-C*pz2) - D) / B);
										firstAbove = copysignf(1, py1 - planey1);
										secondAbove = copysignf(1, py2 - planey2);
									}
									else if (A != 0)
									{
										float planex1 = (((-B*py1) + (-C*pz1) - D) / A);
										float planex2 = (((-B*py2) + (-C*pz2) - D) / A);
										firstAbove = copysignf(1, px1 - planex1);
										secondAbove = copysignf(1, px2 - planex2);
									}
									else if (C != 0)
									{
										float planez1 = (((-B*py1) + (-A*px1) - D) / C);
										float planez2 = (((-B*py2) + (-A*px2) - D) / C);
										firstAbove = copysignf(1, pz1 - planez1);
										secondAbove = copysignf(1, pz2 - planez2);
									}

									if (secondAbove != firstAbove &&
										checkPointInTriangle3D(cix, ciy, ciz, tx1, ty1, tz1, tx2, ty2, tz2, tx3, ty3, tz3,
											fabsf(currTriangle->normal.x),
											fabsf(currTriangle->normal.y),
											fabsf(currTriangle->normal.z)))
									{
										//what is the distance to the triangle? set it to maxdist
										triangleCollide = true;
										float thisDist = (sqrtf(fabsf((cix - px1)*(cix - px1) + (ciy - py1)*(ciy - py1) + (ciz - pz1)*(ciz - pz1))));
										if (minDist == -1 || thisDist < minDist)
										{
											collideTriangle = currTriangle;
											collidePosition.x = cix;
											collidePosition.y = ciy;
											collidePosition.z = ciz;
											minDist = thisDist;
											finalModel = cm;
										}
									}
								}
							}
						}
					}

					//determine the next node
					if (node->depth != treeMaxDepth) //make sure we aren't at a leaf node
					{
						if (px1 < node->xMid)
						{
							if (pz1 < node->zMid)
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
							if (pz1 < node->zMid)
							{
								node = node->botRight;
							}
							else
							{
								node = node->topRight;
							}
						}
					}
					else
					{
						node = nullptr;
					}
				}

				node = cm->quadTreeRoot;

				//search for (px2, pz2)
				while (node != nullptr)
				{
					if (std::find(alreadySearchedNodes.begin(), alreadySearchedNodes.end(), node) == alreadySearchedNodes.end())
					{
						alreadySearchedNodes.push_back(node);

						//check through node.tris;
						for (Triangle3D* currTriangle : node->tris)
						{
							//Bounds check on individual triangle
							if (px1 - checkRadius <= currTriangle->maxX && px1 + checkRadius >= currTriangle->minX &&
								pz1 - checkRadius <= currTriangle->maxZ && pz1 + checkRadius >= currTriangle->minZ &&
								py1 - checkRadius <= currTriangle->maxY && py1 + checkRadius >= currTriangle->minY)
							{
								tx1 = currTriangle->p1X;
								tx2 = currTriangle->p2X;
								tx3 = currTriangle->p3X;
								ty1 = currTriangle->p1Y;
								ty2 = currTriangle->p2Y;
								ty3 = currTriangle->p3Y;
								tz1 = currTriangle->p1Z;
								tz2 = currTriangle->p2Z;
								tz3 = currTriangle->p3Z;
								A = currTriangle->A;
								B = currTriangle->B;
								C = currTriangle->C;
								D = currTriangle->D;

								numerator = (A*px1 + B*py1 + C*pz1 + D);
								denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

								if (denominator != 0)
								{
									u = (numerator / denominator);
									float cix = px1 + u*(px2 - px1);
									float ciy = py1 + u*(py2 - py1);
									float ciz = pz1 + u*(pz2 - pz1);

									if (B != 0)
									{
										float planey1 = (((-A*px1) + (-C*pz1) - D) / B);
										float planey2 = (((-A*px2) + (-C*pz2) - D) / B);
										firstAbove = copysignf(1, py1 - planey1);
										secondAbove = copysignf(1, py2 - planey2);
									}
									else if (A != 0)
									{
										float planex1 = (((-B*py1) + (-C*pz1) - D) / A);
										float planex2 = (((-B*py2) + (-C*pz2) - D) / A);
										firstAbove = copysignf(1, px1 - planex1);
										secondAbove = copysignf(1, px2 - planex2);
									}
									else if (C != 0)
									{
										float planez1 = (((-B*py1) + (-A*px1) - D) / C);
										float planez2 = (((-B*py2) + (-A*px2) - D) / C);
										firstAbove = copysignf(1, pz1 - planez1);
										secondAbove = copysignf(1, pz2 - planez2);
									}

									if (secondAbove != firstAbove &&
										checkPointInTriangle3D(cix, ciy, ciz, tx1, ty1, tz1, tx2, ty2, tz2, tx3, ty3, tz3,
											fabsf(currTriangle->normal.x),
											fabsf(currTriangle->normal.y),
											fabsf(currTriangle->normal.z)))
									{
										//what is the distance to the triangle? set it to maxdist
										triangleCollide = true;
										float thisDist = (sqrtf(fabsf((cix - px1)*(cix - px1) + (ciy - py1)*(ciy - py1) + (ciz - pz1)*(ciz - pz1))));
										if (minDist == -1 || thisDist < minDist)
										{
											collideTriangle = currTriangle;
											collidePosition.x = cix;
											collidePosition.y = ciy;
											collidePosition.z = ciz;
											minDist = thisDist;
											finalModel = cm;
										}
									}
								}
							}
						}
					}

					//determine the next node
					if (node->depth != treeMaxDepth) //make sure we aren't at a leaf node
					{
						if (px2 < node->xMid)
						{
							if (pz2 < node->zMid)
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
							if (pz2 < node->zMid)
							{
								node = node->botRight;
							}
							else
							{
								node = node->topRight;
							}
						}
					}
					else
					{
						node = nullptr;
					}
				}
			}
			else
			{
				for (Triangle3D* currTriangle : cm->triangles)
				{
					//Bounds check on individual triangle
					if (px1 - checkRadius <= currTriangle->maxX && px1 + checkRadius >= currTriangle->minX &&
						pz1 - checkRadius <= currTriangle->maxZ && pz1 + checkRadius >= currTriangle->minZ &&
						py1 - checkRadius <= currTriangle->maxY && py1 + checkRadius >= currTriangle->minY)
					{
						tx1 = currTriangle->p1X;
						tx2 = currTriangle->p2X;
						tx3 = currTriangle->p3X;
						ty1 = currTriangle->p1Y;
						ty2 = currTriangle->p2Y;
						ty3 = currTriangle->p3Y;
						tz1 = currTriangle->p1Z;
						tz2 = currTriangle->p2Z;
						tz3 = currTriangle->p3Z;
						A = currTriangle->A;
						B = currTriangle->B;
						C = currTriangle->C;
						D = currTriangle->D;

						numerator = (A*px1 + B*py1 + C*pz1 + D);
						denominator = (A*(px1 - px2) + B*(py1 - py2) + C*(pz1 - pz2));

						if (denominator != 0)
						{
							u = (numerator / denominator);
							float cix = px1 + u*(px2 - px1);
							float ciy = py1 + u*(py2 - py1);
							float ciz = pz1 + u*(pz2 - pz1);

							if (B != 0)
							{
								float planey1 = (((-A*px1) + (-C*pz1) - D) / B);
								float planey2 = (((-A*px2) + (-C*pz2) - D) / B);
								firstAbove = copysignf(1, py1 - planey1);
								secondAbove = copysignf(1, py2 - planey2);
							}
							else if (A != 0)
							{
								float planex1 = (((-B*py1) + (-C*pz1) - D) / A);
								float planex2 = (((-B*py2) + (-C*pz2) - D) / A);
								firstAbove = copysignf(1, px1 - planex1);
								secondAbove = copysignf(1, px2 - planex2);
							}
							else if (C != 0)
							{
								float planez1 = (((-B*py1) + (-A*px1) - D) / C);
								float planez2 = (((-B*py2) + (-A*px2) - D) / C);
								firstAbove = copysignf(1, pz1 - planez1);
								secondAbove = copysignf(1, pz2 - planez2);
							}

							if (secondAbove != firstAbove &&
								checkPointInTriangle3D(cix, ciy, ciz, tx1, ty1, tz1, tx2, ty2, tz2, tx3, ty3, tz3,
									fabsf(currTriangle->normal.x),
									fabsf(currTriangle->normal.y),
									fabsf(currTriangle->normal.z)))
							{
								//what is the distance to the triangle? set it to maxdist
								triangleCollide = true;
								float thisDist = (sqrtf(fabsf((cix - px1)*(cix - px1) + (ciy - py1)*(ciy - py1) + (ciz - pz1)*(ciz - pz1))));
								if (minDist == -1 || thisDist < minDist)
								{
									collideTriangle = currTriangle;
									collidePosition.x = cix;
									collidePosition.y = ciy;
									collidePosition.z = ciz;
									minDist = thisDist;
									finalModel = cm;
								}
							}
						}
					}
				}
			}
		}
	}
	//CollisionChecker::collideModels.remove(CollisionChecker::stageCollideModel);

	if (CollisionChecker::checkPlayer && finalModel != nullptr)
	{
		finalModel->playerIsOn = true;
	}
	CollisionChecker::checkPlayer = false;

	return triangleCollide;
}

bool CollisionChecker::checkPointInTriangle3D(
	float checkx, float checky, float checkz,
	float cx1,    float cy1,    float cz1,
	float cx2,    float cy2,    float cz2,
	float cx3,    float cy3,    float cz3,
	float nX,     float nY,     float nZ)
{
	if (nY > nX && nY > nZ)
	{
		//from the top
		return (checkPointInTriangle2D(checkx, checkz, cx1, cz1, cx2, cz2, cx3, cz3));
	}
	else if (nX > nZ)
	{
		//from the left
		return (checkPointInTriangle2D(checkz, checky, cz1, cy1, cz2, cy2, cz3, cy3));
	}

	//from the front
	return (checkPointInTriangle2D(checkx, checky, cx1, cy1, cx2, cy2, cx3, cy3));
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


void CollisionChecker::deleteAllCollideModels()
{
	while (CollisionChecker::collideModels.size() > 0)
	{
		CollisionModel* cm = CollisionChecker::collideModels.front();
		CollisionChecker::collideModels.remove(cm);
		cm->deleteMe();
		delete cm; INCR_DEL
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
		delete cm; INCR_DEL
	}
}

void CollisionChecker::deleteCollideModel(CollisionModel* cm)
{
	CollisionChecker::collideModels.remove(cm);
	cm->deleteMe();
	delete cm; INCR_DEL
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
