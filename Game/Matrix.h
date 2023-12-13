#pragma once

class Vector3;

class Matrix
{
public:
	Matrix();
	Matrix(float m00, float m01, float m02, float m03,
		   float m10, float m11, float m12, float m13,
		   float m20, float m21, float m22, float m23,
		   float m30, float m31, float m32, float m33);

	Matrix operator*(const Matrix& rval) const;
	Matrix operator*=(const Matrix& rval);

	Matrix operator*(float scale) const;
	Matrix operator*=(float scale);

	Matrix operator/(float div) const;
	Matrix operator/=(float div);

	// �s��̓]�u
	Matrix Transpose() const;
	Matrix Transpose();

	// ����������炠������֕ϊ������]�s����擾����
	Matrix GetRotationMatrix(const Vector3& from, const Vector3& to);

	// ���s�ړ��s����擾����
	Matrix GetTranslate(const Vector3& v);

	// x����]�s����擾����
	Matrix GetRotationX(float angle);

	// y����]�s����擾����
	Matrix GetRotationY(float angle);

	// z����]�s����擾����
	Matrix GetRotationZ(float angle);

	// �s�񂩂�I�C���[�p�ɕϊ�
	Vector3 ToEulerAngle() const;

public:
	float m[4][4];
};