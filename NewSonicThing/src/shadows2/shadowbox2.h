#ifndef SHADOWS2_H
#define SHADOWS2_H

#include "../toolbox/maths.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"

#include <vector>

/**
* Represents the 3D cuboidal area of the world in which objects will cast
* shadows (basically represents the orthographic projection area for the shadow
* render pass). It is updated each frame to optimise the area, making it as
* small as possible (to allow for optimal shadow map resolution) while not
* being too small to avoid objects not having shadows when they should.
* Everything inside the cuboidal area represented by this object will be
* rendered to the shadow map in the shadow render pass. Everything outside the
* area won't be.
*
* @author Karl
*
*/
class ShadowBox2
{
private:
	const float OFFSET = 35;
	Vector4f UP;
	Vector4f FORWARD;
	const float SHADOW_DISTANCE = 180;

	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	
	Matrix4f* lightViewMatrix;
	Camera* cam;
	
	float farHeight;
	float farWidth;
	float nearHeight;
	float nearWidth;
	
	/**
	* Calculates the position of the vertex at each corner of the view frustum
	* in light space (8 vertices in total, so this returns 8 positions).
	*
	* @param rotation
	*            - camera's rotation.
	* @param forwardVector
	*            - the direction that the camera is aiming, and thus the
	*            direction of the frustum.
	* @param centerNear
	*            - the center point of the frustum's near plane.
	* @param centerFar
	*            - the center point of the frustum's (possibly adjusted) far
	*            plane.
	* @return The positions of the vertices of the frustum in light space.
	*/
	std::vector<Vector4f> calculateFrustumVertices(
		Matrix4f* rotation, Vector3f* forwardVector,
		Vector3f* centerNear, Vector3f* centerFar);
	
	/**
	* Calculates one of the corner vertices of the view frustum in world space
	* and converts it to light space.
	*
	* @param startPoint
	*            - the starting center point on the view frustum.
	* @param direction
	*            - the direction of the corner from the start point.
	* @param width
	*            - the distance of the corner from the start point.
	* @return - The relevant corner vertex of the view frustum in light space.
	*/
	Vector4f calculateLightSpaceFrustumCorner(Vector3f* startPoint, Vector3f* direction, float width);
	
	/**
	* @return The rotation of the camera represented as a matrix.
	*/
	Matrix4f calculateCameraRotationMatrix();
	
	/**
	* Calculates the width and height of the near and far planes of the
	* camera's view frustum. However, this doesn't have to use the "actual" far
	* plane of the view frustum. It can use a shortened view frustum if desired
	* by bringing the far-plane closer, which would increase shadow resolution
	* but means that distant objects wouldn't cast shadows.
	*/
	void calculateWidthsAndHeights();
	
	/**
	* @return The aspect ratio of the display (width:height ratio).
	*/
	float getAspectRatio();

public:
	/**
	* Creates a new shadow box and calculates some initial values relating to
	* the camera's view frustum, namely the width and height of the near plane
	* and (possibly adjusted) far plane.
	*
	* @param lightViewMatrix
	*            - basically the "view matrix" of the light. Can be used to
	*            transform a point from world space into "light" space (i.e.
	*            changes a point's coordinates from being in relation to the
	*            world's axis to being in terms of the light's local axis).
	* @param camera
	*            - the in-game camera.
	*/
	ShadowBox2(Matrix4f* lightViewMatrix, Camera* camera);

	/**
	* Updates the bounds of the shadow box based on the light direction and the
	* camera's view frustum, to make sure that the box covers the smallest area
	* possible while still ensuring that everything inside the camera's view
	* (within a certain range) will cast shadows.
	*/
	void update();

	/**
	* Calculates the center of the "view cuboid" in light space first, and then
	* converts this to world space using the inverse light's view matrix.
	*
	* @return The center of the "view cuboid" in world space.
	*/
	Vector3f getCenter();

	/**
	* @return The width of the "view cuboid" (orthographic projection area).
	*/
	float getWidth();

	/**
	* @return The height of the "view cuboid" (orthographic projection area).
	*/
	float getHeight();
	
	/**
	* @return The length of the "view cuboid" (orthographic projection area).
	*/
	float getLength();
};
#endif