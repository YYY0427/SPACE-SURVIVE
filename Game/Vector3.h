#pragma once

class Matrix;

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);

	// Vec3 = Vec3 + Vec3
	Vector3 operator+ (const Vector3& rval) const;

	// Vec3 = Vec3 - Vec3
	Vector3 operator- (const Vector3& rval) const;

	// Vec3 = (Vec3 += Vec3)
	void operator+= (const Vector3& rval);

	// Vec3 = (Vec3 -= Vec3)
	void operator-= (const Vector3& rval);

	// Vec3 = Vec3 * float
	Vector3 operator*(float scale)const;

	// Vec3 = Vec3 / floats
	Vector3 operator/(float div)const;

	// Vec3 = (Vec3 *= float)
	void operator*=(float scale);

	// Vec3 = (Vec3 /= float)
	void operator/=(float div);

	// �x�N�^�[�t�](-)
	Vector3 operator-() const;

	// 
	bool operator==(const Vector3& vec);

	// 
	bool operator!=(const Vector3& vec);

	/// �x�N�g���̑傫����Ԃ�
	float Length() const;

	// �x�N�g���̑傫����2���Ԃ�
    float SQLength() const;

	// �x�N�g���̐��K��
	void Normalize();

	// ���K�������x�N�g����Ԃ�
	Vector3 Normalized() const;

	// �O��
	Vector3 Cross(const Vector3& v1, const Vector3& v2) const;

	// ����
	float Dot(const Vector3& v1, const Vector3& v2) const;

	// ���˃x�N�g��
	Vector3 Reflect(const Vector3& vec, const Vector3& normal) const;

	// �X���C�h�x�N�g��
	Vector3 Slide(const Vector3& vec, const Vector3& normal) const;

	// ���`���
	Vector3 Lerp(const Vector3& start, const Vector3& end, float value) const;

	// �G���~�[�g���
	Vector3 Hermite(const Vector3& startPos, const Vector3& startTangent, const Vector3& endPos, const Vector3& endTangent, float value) const;
	
	// �s����g�����x�N�g���̕ϊ�
	Vector3 Transform(const Vector3& vec, const Matrix& mat) const;

public:
	float x, y, z;
};