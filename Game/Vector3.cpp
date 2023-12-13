#include "Vector3.h"
#include "Matrix.h"
#include <cmath>

Vector3::Vector3() :
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Vector3::Vector3(float x, float y, float z) :
	x(x),
	y(y),
	z(z)
{
}

// Vec3 = Vec3 + Vec3
Vector3 Vector3::operator+(const Vector3& rval) const
{
	return { x + rval.x, y + rval.y, z + rval.z };
}

// Vec3 = Vec3 - Vec3
Vector3 Vector3::operator-(const Vector3& rval) const
{
	return { x - rval.x, y - rval.y, z - rval.z };
}

// Vec3 = (Vec3 += Vec3)
void Vector3::operator+=(const Vector3& rval)
{
	x += rval.x;
	y += rval.y;
	z += rval.z;
}

// Vec3 = (Vec3 -= Vec3)
void Vector3::operator-=(const Vector3& rval)
{
	x -= rval.x;
	y -= rval.y;
	z -= rval.z;
}

// Vec3 = Vec3 * float
Vector3 Vector3::operator*(float scale) const
{
	return { x * scale, y * scale, z * scale };
}

// Vec3 = Vec3 / float
Vector3 Vector3::operator/(float div) const
{
	return { x / div, y / div, z / div };
}

// Vec3 = (Vec3 *= float)
void Vector3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

// Vec3 = (Vec3 /= float)
void Vector3::operator/=(float div)
{
	x /= div;
	y /= div;
	z /= div;
}

// Vec3 = -Vec3
Vector3 Vector3::operator-()const
{
	return { -x, -y, -z };
}

bool Vector3::operator==(const Vector3& vec)
{
	return (x == vec.x && y == vec.y && z == vec.z);
}

bool Vector3::operator!=(const Vector3& vec)
{
	return (x != vec.x || y != vec.y || z != vec.z);
}

/// �x�N�g���̑傫����Ԃ�
float Vector3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

// �x�N�g���̑傫����2���Ԃ�
float Vector3::SQLength() const
{
	return x * x + y * y + z * z;
}

// �x�N�g���̐��K��
void Vector3::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	z /= length;
}

// ���K�������x�N�g����Ԃ�
Vector3 Vector3::Normalized() const
{
	float length = Length();
	return { x / length, y / length, z / length };
}

// �x�N�g���̓��ς�Ԃ�
float Vector3::Dot(const Vector3& v1, const Vector3& v2) const 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// �x�N�g���̊O�ς�Ԃ�
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) const
{
	return { v1.y * v2.z - v1.z * v2.y,
			 v1.z * v2.x - v1.x * v2.z,
			 v1.x * v2.y - v1.y * v2.x };
}

// ���˃x�N�g����Ԃ�
Vector3 Vector3::Reflect(const Vector3& vec, const Vector3& normal) const
{
	return vec - normal * 2.0f * Dot(vec, normal);
}

// �X���C�h�x�N�g����Ԃ�
Vector3 Vector3::Slide(const Vector3& vec, const Vector3& normal) const
{
	return vec - normal * Dot(vec, normal);
}

// ���`���
Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float value) const
{
	return start + (end - start) * value;
}

// �G���~�[�g���
Vector3 Vector3::Hermite(const Vector3& startPos, const Vector3& startTangent, const Vector3& endPos, const Vector3& endTangent, float value) const
{
	float t2_3 = value * value * value;
	float t3_2 = value * value;
	float h1 = 2.0f * t2_3 - 3.0f * t3_2 + 1.0f;
	float h2 = -2.0f * t2_3 + 3.0f * t3_2;
	float h3 = t2_3 - 2.0f * t3_2 + value;
	float h4 = t2_3 - t3_2;

	return startPos * h1 + endPos * h2 + startTangent * h3 + endTangent * h4;
}

// �s����g�����x�N�g���̕ϊ�
Vector3 Vector3::Transform(const Vector3& vec, const Matrix& mat) const
{
	return { vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0],
			 vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1],
			 vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2] };
}