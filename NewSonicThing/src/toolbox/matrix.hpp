#ifndef MATRIX_H
#define MATRIX_H

class Vector3f;

#include <math.h>
#include "vector.hpp"

class Matrix4f
{
public:
    float m00;
    float m01;
    float m02;
    float m03;
    float m10;
    float m11;
    float m12;
    float m13;
    float m20;
    float m21;
    float m22;
    float m23;
    float m30;
    float m31;
    float m32;
    float m33;

    // defaults to identity matrix
    Matrix4f();

    // wont set any of the values (regardless of parameter)
    Matrix4f(bool uninitialized);

    // copies values of other
    Matrix4f(Matrix4f* other);

    void setZero();

    void setIdentity();

    /**
    * Copy the buffered matrix into this matrix
    * @param buf The buffered matrix data
    */
    void load(float[]);

    /**
    *
    * @param buf Buffer to put this matrix data into
    */
    void store(float[]);

    /**
    * Multiply the right matrix by this matrix and place the result in a dest matrix.
    * right and dest can be the same matrix and it wont break.
    * @param right The right source matrix
    * @param dest The destination matrix
    */
    void multiply(Matrix4f* right, Matrix4f* dest);

    void translate(Vector3f*);

    void translate(Vector2f*);

    void scale(Vector3f*);

    void scale(Vector2f*);

    void scale(float);

    void set(Matrix4f* other);

    //angle in radians
    void rotate(float radians, const Vector3f* axis);

    Vector4f transform(Vector4f* vec);

    void invert();

    float determinant();

    float determinant3x3(
        float t00, float t01, float t02,
        float t10, float t11, float t12,
        float t20, float t21, float t22);
};
#endif
