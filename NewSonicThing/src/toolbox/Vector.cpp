#include <cmath>

#include "vector.hpp"

void Vector3f::set(float newX, float newY, float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

void Vector3f::set(const Vector3f* other)
{
    x = other->x;
    y = other->y;
    z = other->z;
}

float Vector3f::length() const
{
    return sqrtf(x*x + y*y + z*z);
}

float Vector3f::lengthSquared() const
{
    return (x * x) + (y * y) + (z * z);
}

void Vector3f::setLength(float newLength)
{
    float currLength = length();
    if (currLength > 0.0000001f)
    {
        float ratio = newLength/currLength;
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }
    else 
    {
        //printf("Warning: Trying to set length of a very small vector [%f %f %f]\n", x, y, z);
        float xa = std::abs(x);
        float ya = std::abs(y);
        float max = fmaxf(xa, fmaxf(ya, std::abs(z)));
        if (xa == max)
        {
            y = 0;
            z = 0;
            if (x > 0)
            {
                x = newLength;
            }
            else
            {
                x = -newLength;
            }
        }
        else if (ya == max)
        {
            x = 0;
            z = 0;
            if (y > 0)
            {
                y = newLength;
            }
            else
            {
                y = -newLength;
            }
        }
        else
        {
            x = 0;
            y = 0;
            if (z > 0)
            {
                z = newLength;
            }
            else
            {
                z = -newLength;
            }
        }
    }
}

void Vector3f::normalize()
{
    float mag = length();

    if (mag > 0.0000001f)
    {
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }
    else
    {
        //printf("Warning: Trying to normalize a very small vector [%f %f %f]\n", x, y, z);
        float xa = std::abs(x);
        float ya = std::abs(y);
        float max = fmaxf(xa, fmaxf(ya, std::abs(z)));
        if (xa == max)
        {
            y = 0;
            z = 0;
            if (x > 0)
            {
                x = 1;
            }
            else
            {
                x = -1;
            }
        }
        else if (ya == max)
        {
            x = 0;
            z = 0;
            if (y > 0)
            {
                y = 1;
            }
            else
            {
                y = -1;
            }
        }
        else
        {
            x = 0;
            y = 0;
            if (z > 0)
            {
                z = 1;
            }
            else
            {
                z = -1;
            }
        }
    }
}

void Vector3f::neg()
{
    x = -x;
    y = -y;
    z = -z;
}

void Vector3f::scale(float scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
}

Vector3f Vector3f::scaleCopy(float scale) const
{
    return Vector3f(x*scale, y*scale, z*scale);
}

float Vector3f::dot(const Vector3f* other) const
{
    return x * other->x + y * other->y + z * other->z;
}

Vector3f Vector3f::cross(const Vector3f* other) const
{
    float x_ = y * other->z - z * other->y;
    float y_ = z * other->x - x * other->z;
    float z_ = x * other->y - y * other->x;

    return Vector3f(x_, y_, z_);
}

Vector3f Vector3f::operator + (const Vector3f &other)
{
    return Vector3f(x + other.x, y + other.y, z + other.z);
}

Vector3f Vector3f::operator - (const Vector3f &other)
{
    return Vector3f(x - other.x, y - other.y, z - other.z);
}

Vector3f Vector3f::operator * (const Vector3f &other)
{
    return Vector3f(x * other.x, y * other.y, z * other.z);
}

Vector3f Vector3f::operator / (const Vector3f &other)
{
    return Vector3f(x / other.x, y / other.y, z / other.z);
}

bool Vector3f::operator == (const Vector3f &other)
{
    return (x == other.x && y == other.y && z == other.z);
}

Vector3f::Vector3f()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3f::Vector3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3f::Vector3f(const Vector3f* base)
{
    this->x = base->x;
    this->y = base->y;
    this->z = base->z;
}






void Vector2f::set(float newX, float newY)
{
    x = newX;
    y = newY;
}

void Vector2f::set(Vector2f* vec)
{
    x = vec->x;
    y = vec->y;
}

float Vector2f::length() const
{
    return sqrtf((x * x) + (y * y));
}

float Vector2f::lengthSquared() const
{
    return (x * x) + (y * y);
}

Vector2f Vector2f::normalized() const
{
    float mag = length();

    return Vector2f(x / mag, y / mag);
}

void Vector2f::neg()
{
    x = -x;
    y = -y;
}

float Vector2f::dot(Vector2f* other) const
{
    return x * other->x + y * other->y;
}

Vector2f Vector2f::scaleCopy(float scale) const
{
    return Vector2f(x*scale, y*scale);
}

Vector2f Vector2f::operator + (const Vector2f &other)
{
    return Vector2f(x + other.x, y + other.y);
}

Vector2f Vector2f::operator - (const Vector2f &other)
{
    return Vector2f(x - other.x, y - other.y);
}

Vector2f Vector2f::operator * (const Vector2f &other)
{
    return Vector2f(x * other.x, y * other.y);
}

Vector2f Vector2f::operator / (const Vector2f &other)
{
    return Vector2f(x / other.x, y / other.y);
}

Vector2f Vector2f::operator * (const float &scale)
{
    return Vector2f(x * scale, y * scale);
}

Vector2f::Vector2f()
{
    x = 0;
    y = 0;
}

Vector2f::Vector2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2f::Vector2f(Vector2f* base)
{
    this->x = base->x;
    this->y = base->y;
}
















void Vector4f::set(float newX, float newY, float newZ, float newW)
{
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
}

void Vector4f::set(Vector4f* other)
{
    x = other->x;
    y = other->y;
    z = other->z;
    w = other->w;
}

float Vector4f::length() const
{
    return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
}

float Vector4f::lengthSquared() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

void Vector4f::normalize()
{
    float mag = length();

    x = x / mag;
    y = y / mag;
    z = z / mag;
    w = w / mag;
}

void Vector4f::neg()
{
    x = -x;
    y = -y;
    z = -z;
    w = -w;
}

void Vector4f::scale(float scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
}

float Vector4f::dot(Vector4f* other) const
{
    return x * other->x + y * other->y + z * other->z + w * other->w;
}

Vector4f Vector4f::operator + (const Vector4f &other)
{
    return Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4f Vector4f::operator - (const Vector4f &other)
{
    return Vector4f(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4f Vector4f::operator * (const Vector4f &other)
{
    return Vector4f(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4f Vector4f::operator / (const Vector4f &other)
{
    return Vector4f(x / other.x, y / other.y, z / other.z, w / other.w);
}


Vector4f::Vector4f()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4f::Vector4f(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4f::Vector4f(Vector4f* base)
{
    x = base->x;
    y = base->y;
    z = base->z;
    w = base->w;
}
