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

	// 行列の転置
	Matrix Transpose() const;
	Matrix Transpose();

	// ある向きからある向きへ変換する回転行列を取得する
	Matrix GetRotationMatrix(const Vector3& from, const Vector3& to);

	// 平行移動行列を取得する
	Matrix GetTranslate(const Vector3& v);

	// x軸回転行列を取得する
	Matrix GetRotationX(float angle);

	// y軸回転行列を取得する
	Matrix GetRotationY(float angle);

	// z軸回転行列を取得する
	Matrix GetRotationZ(float angle);

	// 行列からオイラー角に変換
	Vector3 ToEulerAngle() const;

public:
	float m[4][4];
};