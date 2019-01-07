#include <math.h>

#include "matrix.h"
#include "vector.h"

Matrix4f::Matrix4f()
{
	setIdentity();
}

Matrix4f::Matrix4f(Matrix4f* base)
{
	this->m00 = base->m00;
	this->m01 = base->m01;
	this->m02 = base->m02;
	this->m03 = base->m03;
	this->m10 = base->m10;
	this->m11 = base->m11;
	this->m12 = base->m12;
	this->m13 = base->m13;
	this->m20 = base->m20;
	this->m21 = base->m21;
	this->m22 = base->m22;
	this->m23 = base->m23;
	this->m30 = base->m30;
	this->m31 = base->m31;
	this->m32 = base->m32;
	this->m33 = base->m33;
}

void Matrix4f::setZero()
{
	m00 = 0.0f;
	m01 = 0.0f;
	m02 = 0.0f;
	m03 = 0.0f;
	m10 = 0.0f;
	m11 = 0.0f;
	m12 = 0.0f;
	m13 = 0.0f;
	m20 = 0.0f;
	m21 = 0.0f;
	m22 = 0.0f;
	m23 = 0.0f;
	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m33 = 0.0f;
}

void Matrix4f::setIdentity()
{
	m00 = 1.0f;
	m01 = 0.0f;
	m02 = 0.0f;
	m03 = 0.0f;
	m10 = 0.0f;
	m11 = 1.0f;
	m12 = 0.0f;
	m13 = 0.0f;
	m20 = 0.0f;
	m21 = 0.0f;
	m22 = 1.0f;
	m23 = 0.0f;
	m30 = 0.0f;
	m31 = 0.0f;
	m32 = 0.0f;
	m33 = 1.0f;
}

/**
* Copy the buffered matrix into this matrix
* @param buf The buffered matrix data
*/
void Matrix4f::load(float buf[])
{
	m00 = buf[0];
	m01 = buf[1];
	m02 = buf[2];
	m03 = buf[3];
	m10 = buf[4];
	m11 = buf[5];
	m12 = buf[6];
	m13 = buf[7];
	m20 = buf[8];
	m21 = buf[9];
	m22 = buf[10];
	m23 = buf[11];
	m30 = buf[12];
	m31 = buf[13];
	m32 = buf[14];
	m33 = buf[15];
}

/**
*
* @param buf Buffer to put this matrix data into
*/
void Matrix4f::store(float buf[])
{
	buf[0] = m00;
	buf[1] = m01;
	buf[2] = m02;
	buf[3] = m03;
	buf[4] = m10;
	buf[5] = m11;
	buf[6] = m12;
	buf[7] = m13;
	buf[8] = m20;
	buf[9] = m21;
	buf[10] = m22;
	buf[11] = m23;
	buf[12] = m30;
	buf[13] = m31;
	buf[14] = m32;
	buf[15] = m33;
}

void Matrix4f::set(Matrix4f* other)
{
	m00 = other->m00;
	m01 = other->m01;
	m02 = other->m02;
	m03 = other->m03;
	m10 = other->m10;
	m11 = other->m11;
	m12 = other->m12;
	m13 = other->m13;
	m20 = other->m20;
	m21 = other->m21;
	m22 = other->m22;
	m23 = other->m23;
	m30 = other->m30;
	m31 = other->m31;
	m32 = other->m32;
	m33 = other->m33;
}

/**
* Multiply the right matrix by this matrix and place the result in a dest matrix.
* @param right The right source matrix
* @param dest The destination matrix
*/
void Matrix4f::multiply(Matrix4f* right, Matrix4f* dest)
{
	float newm00 = this->m00 * right->m00 + this->m10 * right->m01 + this->m20 * right->m02 + this->m30 * right->m03;
	float newm01 = this->m01 * right->m00 + this->m11 * right->m01 + this->m21 * right->m02 + this->m31 * right->m03;
	float newm02 = this->m02 * right->m00 + this->m12 * right->m01 + this->m22 * right->m02 + this->m32 * right->m03;
	float newm03 = this->m03 * right->m00 + this->m13 * right->m01 + this->m23 * right->m02 + this->m33 * right->m03;
	float newm10 = this->m00 * right->m10 + this->m10 * right->m11 + this->m20 * right->m12 + this->m30 * right->m13;
	float newm11 = this->m01 * right->m10 + this->m11 * right->m11 + this->m21 * right->m12 + this->m31 * right->m13;
	float newm12 = this->m02 * right->m10 + this->m12 * right->m11 + this->m22 * right->m12 + this->m32 * right->m13;
	float newm13 = this->m03 * right->m10 + this->m13 * right->m11 + this->m23 * right->m12 + this->m33 * right->m13;
	float newm20 = this->m00 * right->m20 + this->m10 * right->m21 + this->m20 * right->m22 + this->m30 * right->m23;
	float newm21 = this->m01 * right->m20 + this->m11 * right->m21 + this->m21 * right->m22 + this->m31 * right->m23;
	float newm22 = this->m02 * right->m20 + this->m12 * right->m21 + this->m22 * right->m22 + this->m32 * right->m23;
	float newm23 = this->m03 * right->m20 + this->m13 * right->m21 + this->m23 * right->m22 + this->m33 * right->m23;
	float newm30 = this->m00 * right->m30 + this->m10 * right->m31 + this->m20 * right->m32 + this->m30 * right->m33;
	float newm31 = this->m01 * right->m30 + this->m11 * right->m31 + this->m21 * right->m32 + this->m31 * right->m33;
	float newm32 = this->m02 * right->m30 + this->m12 * right->m31 + this->m22 * right->m32 + this->m32 * right->m33;
	float newm33 = this->m03 * right->m30 + this->m13 * right->m31 + this->m23 * right->m32 + this->m33 * right->m33;

	dest->m00 = newm00;
	dest->m01 = newm01;
	dest->m02 = newm02;
	dest->m03 = newm03;
	dest->m10 = newm10;
	dest->m11 = newm11;
	dest->m12 = newm12;
	dest->m13 = newm13;
	dest->m20 = newm20;
	dest->m21 = newm21;
	dest->m22 = newm22;
	dest->m23 = newm23;
	dest->m30 = newm30;
	dest->m31 = newm31;
	dest->m32 = newm32;
	dest->m33 = newm33;
}

void Matrix4f::translate(Vector3f* vec)
{
	m30 += m00 * vec->x + m10 * vec->y + m20 * vec->z;
	m31 += m01 * vec->x + m11 * vec->y + m21 * vec->z;
	m32 += m02 * vec->x + m12 * vec->y + m22 * vec->z;
	m33 += m03 * vec->x + m13 * vec->y + m23 * vec->z;
}

void Matrix4f::translate(Vector2f* vec)
{
	m30 += m00 * vec->x + m10 * vec->y;
	m31 += m01 * vec->x + m11 * vec->y;
	m32 += m02 * vec->x + m12 * vec->y;
	m33 += m03 * vec->x + m13 * vec->y;
}

void Matrix4f::scale(Vector3f* vec)
{
	m00 = m00 * vec->x;
	m01 = m01 * vec->x;
	m02 = m02 * vec->x;
	m03 = m03 * vec->x;
	m10 = m10 * vec->y;
	m11 = m11 * vec->y;
	m12 = m12 * vec->y;
	m13 = m13 * vec->y;
	m20 = m20 * vec->z;
	m21 = m21 * vec->z;
	m22 = m22 * vec->z;
	m23 = m23 * vec->z;
}

void Matrix4f::scale(Vector2f* vec)
{
	m00 = m00 * vec->x;
	m01 = m01 * vec->x;
	m02 = m02 * vec->x;
	m03 = m03 * vec->x;
	m10 = m10 * vec->y;
	m11 = m11 * vec->y;
	m12 = m12 * vec->y;
	m13 = m13 * vec->y;
}

//angle in radians
void Matrix4f::rotate(float angle, Vector3f* axis)
{
	float c = (float)cos(angle);
	float s = (float)sin(angle);
	float oneminusc = 1.0f - c;
	float xy = axis->x*axis->y;
	float yz = axis->y*axis->z;
	float xz = axis->x*axis->z;
	float xs = axis->x*s;
	float ys = axis->y*s;
	float zs = axis->z*s;

	float f00 = axis->x*axis->x*oneminusc + c;
	float f01 = xy*oneminusc + zs;
	float f02 = xz*oneminusc - ys;
	// n[3] not used
	float f10 = xy*oneminusc - zs;
	float f11 = axis->y*axis->y*oneminusc + c;
	float f12 = yz*oneminusc + xs;
	// n[7] not used
	float f20 = xz*oneminusc + ys;
	float f21 = yz*oneminusc - xs;
	float f22 = axis->z*axis->z*oneminusc + c;

	float t00 = m00 * f00 + m10 * f01 + m20 * f02;
	float t01 = m01 * f00 + m11 * f01 + m21 * f02;
	float t02 = m02 * f00 + m12 * f01 + m22 * f02;
	float t03 = m03 * f00 + m13 * f01 + m23 * f02;
	float t10 = m00 * f10 + m10 * f11 + m20 * f12;
	float t11 = m01 * f10 + m11 * f11 + m21 * f12;
	float t12 = m02 * f10 + m12 * f11 + m22 * f12;
	float t13 = m03 * f10 + m13 * f11 + m23 * f12;
	m20 = m00 * f20 + m10 * f21 + m20 * f22;
	m21 = m01 * f20 + m11 * f21 + m21 * f22;
	m22 = m02 * f20 + m12 * f21 + m22 * f22;
	m23 = m03 * f20 + m13 * f21 + m23 * f22;
	m00 = t00;
	m01 = t01;
	m02 = t02;
	m03 = t03;
	m10 = t10;
	m11 = t11;
	m12 = t12;
	m13 = t13;
}

Vector4f Matrix4f::transform(Vector4f* vec)
{
	float x = m00 * vec->x + m10 * vec->y + m20 * vec->z + m30 * vec->w;
	float y = m01 * vec->x + m11 * vec->y + m21 * vec->z + m31 * vec->w;
	float z = m02 * vec->x + m12 * vec->y + m22 * vec->z + m32 * vec->w;
	float w = m03 * vec->x + m13 * vec->y + m23 * vec->z + m33 * vec->w;

	return Vector4f(x, y, z, w);
}

void Matrix4f::invert()
{
	float deter = determinant();

	if (deter != 0)
	{
		/*
		* m00 m01 m02 m03
		* m10 m11 m12 m13
		* m20 m21 m22 m23
		* m30 m31 m32 m33
		*/
		float determinant_inv = 1.0f / deter;

		// first row
		float t00 =  determinant3x3(m11, m12, m13, m21, m22, m23, m31, m32, m33);
		float t01 = -determinant3x3(m10, m12, m13, m20, m22, m23, m30, m32, m33);
		float t02 =  determinant3x3(m10, m11, m13, m20, m21, m23, m30, m31, m33);
		float t03 = -determinant3x3(m10, m11, m12, m20, m21, m22, m30, m31, m32);
		// second row
		float t10 = -determinant3x3(m01, m02, m03, m21, m22, m23, m31, m32, m33);
		float t11 =  determinant3x3(m00, m02, m03, m20, m22, m23, m30, m32, m33);
		float t12 = -determinant3x3(m00, m01, m03, m20, m21, m23, m30, m31, m33);
		float t13 =  determinant3x3(m00, m01, m02, m20, m21, m22, m30, m31, m32);
		// third row
		float t20 =  determinant3x3(m01, m02, m03, m11, m12, m13, m31, m32, m33);
		float t21 = -determinant3x3(m00, m02, m03, m10, m12, m13, m30, m32, m33);
		float t22 =  determinant3x3(m00, m01, m03, m10, m11, m13, m30, m31, m33);
		float t23 = -determinant3x3(m00, m01, m02, m10, m11, m12, m30, m31, m32);
		// fourth row
		float t30 = -determinant3x3(m01, m02, m03, m11, m12, m13, m21, m22, m23);
		float t31 =  determinant3x3(m00, m02, m03, m10, m12, m13, m20, m22, m23);
		float t32 = -determinant3x3(m00, m01, m03, m10, m11, m13, m20, m21, m23);
		float t33 =  determinant3x3(m00, m01, m02, m10, m11, m12, m20, m21, m22);

		// transpose and divide by the determinant
		m00 = t00*determinant_inv;
		m11 = t11*determinant_inv;
		m22 = t22*determinant_inv;
		m33 = t33*determinant_inv;
		m01 = t10*determinant_inv;
		m10 = t01*determinant_inv;
		m20 = t02*determinant_inv;
		m02 = t20*determinant_inv;
		m12 = t21*determinant_inv;
		m21 = t12*determinant_inv;
		m03 = t30*determinant_inv;
		m30 = t03*determinant_inv;
		m13 = t31*determinant_inv;
		m31 = t13*determinant_inv;
		m32 = t23*determinant_inv;
		m23 = t32*determinant_inv;
	}
}

float Matrix4f::determinant()
{
	float f = m00 * ((m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32)
			        - m13 * m22 * m31
			        - m11 * m23 * m32
			        - m12 * m21 * m33);

	f -= m01 * ((m10 * m22 * m33 + m12 * m23 * m30 + m13 * m20 * m32)
			   - m13 * m22 * m30
			   - m10 * m23 * m32
			   - m12 * m20 * m33);

	f += m02 * ((m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31)
			   - m13 * m21 * m30
			   - m10 * m23 * m31
			   - m11 * m20 * m33);

	f -= m03 * ((m10 * m21 * m32 + m11 * m22 * m30 + m12 * m20 * m31)
			   - m12 * m21 * m30
			   - m10 * m22 * m31
			   - m11 * m20 * m32);

	return f;
}

float Matrix4f::determinant3x3(
	float t00, float t01, float t02,
	float t10, float t11, float t12,
	float t20, float t21, float t22)
{
	return t00 * (t11 * t22 - t12 * t21)
		 + t01 * (t12 * t20 - t10 * t22)
		 + t02 * (t10 * t21 - t11 * t20);
}