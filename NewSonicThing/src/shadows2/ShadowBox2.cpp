#include "shadowbox2.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/vector.hpp"
#include "../toolbox/matrix.hpp"
#include "../entities/camera.hpp"
#include "../renderEngine/display.hpp"
#include "../renderEngine/masterrenderer.hpp"

#include <vector>

ShadowBox2::ShadowBox2(Matrix4f* lightViewMatrix, Camera* camera)
{
    UP = Vector4f(0, 1, 0, 0);
    FORWARD = Vector4f(0, 0, -1, 0);

    this->lightViewMatrix = lightViewMatrix;
    this->cam = camera;
    calculateWidthsAndHeights();
}

void ShadowBox2::update()
{
    Matrix4f rotation = calculateCameraRotationMatrix();
    Vector4f temp = rotation.transform(&FORWARD);
    Vector3f forwardVector(temp.x, temp.y, temp.z);

    Vector3f toFar(&forwardVector);
    toFar.scale(SHADOW_DISTANCE);

    Vector3f toNear(&forwardVector);
    toNear.scale(MasterRenderer::NEAR_PLANE);

    Vector3f centerNear = toNear + cam->eye;
    Vector3f centerFar  = toFar  + cam->eye;

    std::vector<Vector4f> points = calculateFrustumVertices(
        &rotation, &forwardVector, &centerNear, &centerFar);

    bool first = true;
    for (Vector4f point : points) 
    {
        if (first) 
        {
            minX = point.x;
            maxX = point.x;
            minY = point.y;
            maxY = point.y;
            minZ = point.z;
            maxZ = point.z;
            first = false;
            continue;
        }
            
        if (point.x > maxX) 
        {
            maxX = point.x;
        } 
        else if (point.x < minX) 
        {
            minX = point.x;
        }
            
        if (point.y > maxY) 
        {
            maxY = point.y;
        } 
        else if (point.y < minY) 
        {
            minY = point.y;
        }
            
        if (point.z > maxZ) 
        {
            maxZ = point.z;
        } 
        else if (point.z < minZ) 
        {
            minZ = point.z;
        }
    }
    maxZ += OFFSET;
}

Vector3f ShadowBox2::getCenter()
{
    float x = (minX + maxX) / 2.0f;
    float y = (minY + maxY) / 2.0f;
    float z = (minZ + maxZ) / 2.0f;
    Vector4f cen(x, y, z, 1);
    Matrix4f invertedLight(lightViewMatrix);
    invertedLight.invert();
    Vector4f temp = invertedLight.transform(&cen);
    return Vector3f(temp.x, temp.y, temp.z);
}

float ShadowBox2::getWidth()
{
    return maxX - minX;
}

float ShadowBox2::getHeight()
{
    return maxY - minY;
}

float ShadowBox2::getLength()
{
    return maxZ - minZ;
}

std::vector<Vector4f> ShadowBox2::calculateFrustumVertices(
    Matrix4f* rotation, Vector3f* forwardVector,
    Vector3f* centerNear, Vector3f* centerFar)
{
    Matrix4f rot(rotation);
    Vector4f temp = rot.transform(&UP);
    Vector3f upVector(temp.x, temp.y, temp.z);

    Vector3f rightVector = forwardVector->cross(&upVector);

    Vector3f downVector(-upVector.x, -upVector.y, -upVector.z);
    Vector3f leftVector(-rightVector.x, -rightVector.y, -rightVector.z);


    Vector3f temp2(upVector.x * farHeight, upVector.y * farHeight, upVector.z * farHeight);
    Vector3f farTop = (*centerFar) + temp2;

    temp2.set(downVector.x * farHeight, downVector.y * farHeight, downVector.z * farHeight);
    Vector3f farBottom = (*centerFar) + temp2;

    temp2.set(upVector.x * nearHeight, upVector.y * nearHeight, upVector.z * nearHeight);
    Vector3f nearTop = (*centerNear) + temp2;

    temp2.set(downVector.x * nearHeight, downVector.y * nearHeight, downVector.z * nearHeight);
    Vector3f nearBottom = (*centerNear) + temp2;

    std::vector<Vector4f> points;

    points.push_back(calculateLightSpaceFrustumCorner(&farTop,     &rightVector, farWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&farTop,     &leftVector,  farWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&farBottom,  &rightVector, farWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&farBottom,  &leftVector,  farWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&nearTop,    &rightVector, nearWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&nearTop,    &leftVector,  nearWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&nearBottom, &rightVector, nearWidth));
    points.push_back(calculateLightSpaceFrustumCorner(&nearBottom, &leftVector,  nearWidth));
    return points;
}

Vector4f ShadowBox2::calculateLightSpaceFrustumCorner(Vector3f* startPoint, Vector3f* direction, float width)
{
    Vector3f temp(direction->x * width, direction->y * width, direction->z * width);
    Vector3f point = (*startPoint) + temp;
    Vector4f point4f(point.x, point.y, point.z, 1.0f);

    return lightViewMatrix->transform(&point4f);
}

Matrix4f ShadowBox2::calculateCameraRotationMatrix()
{
    Matrix4f rotation;
    Vector3f temp(0, 1, 0);
    //rotation.rotate(Maths::toRadians(-cam->getYaw()), &temp);
    temp.set(1, 0, 0);
    rotation.rotate(Maths::toRadians(-90), &temp);
    return rotation;
}

void ShadowBox2::calculateWidthsAndHeights()
{
    float VFOV = MasterRenderer::getVFOV();

    float aRatio = getAspectRatio();

    farWidth = SHADOW_DISTANCE * tanf(Maths::toRadians(VFOV));
    nearWidth = MasterRenderer::NEAR_PLANE * tanf(Maths::toRadians(VFOV));
    farHeight = farWidth / aRatio;
    nearHeight = nearWidth / aRatio;
}

float ShadowBox2::getAspectRatio()
{
    return Display::ASPECT_RATIO;
}
