#include <cassert>
#include <cmath>
#include "MathUtil.h"

VECTOR MathUtil::ToEulerAngles(const MATRIX mtx, bool& isGimbalLock, RotationOrder order)
{
	float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;

	switch (order)
	{
	case RotationOrder::XYZ:
		isGimbalLock = EulerAnglesXYZ(mtx, xRot, yRot, zRot);
		break;
	case RotationOrder::YZX:
		isGimbalLock = EulerAnglesYZX(mtx, xRot, yRot, zRot);
		break;
	case RotationOrder::ZXY:
		isGimbalLock = EulerAnglesZXY(mtx, xRot, yRot, zRot);
		break;
	default:
		assert(0);
	}
	return { xRot, yRot, zRot };
}

float MathUtil::RadianFromDegree(float radian)
{
	return radian * 180.0f / DX_PI_F;
}

float MathUtil::DegreeFromRadian(float degree)
{
	return degree * DX_PI_F / 180.0f;
}

bool MathUtil::EulerAnglesXYZ(const MATRIX mat, float& xRot, float& yRot, float& zRot)
{
	const float Threshold = 0.0001f;

	// ジンバルロック判定
	if (mat.m[0][2] > 1.0f - Threshold || mat.m[0][2] < -1.0f + Threshold)
	{
		xRot = 0.0f;
		yRot = (mat.m[0][2] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		zRot = -atan2f(-mat.m[1][0], mat.m[1][1]);
		return false;
	}

	yRot = -asinf(mat.m[0][2]);
	xRot = asinf(mat.m[1][2] / cosf(yRot));

	// ジンバルロック判定
	if ((isnan(xRot) != 0))
	{
		xRot = 0.0f;
		yRot = (mat.m[0][2] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		zRot = -atan2f(-mat.m[1][0], mat.m[1][1]);
		return false;
	}

	if (mat.m[2][2] < 0.0f)
	{
		xRot = DX_PI_F - xRot;
	}

	zRot = atan2f(mat.m[0][1], mat.m[0][0]);
	return true;
}

bool MathUtil::EulerAnglesYZX(const MATRIX mat, float& xRot, float& yRot, float& zRot)
{
	const float Threshold = 0.0001f;

	// ジンバルロック判定
	if (mat.m[1][0] > 1.0f - Threshold || mat.m[1][0] < -1.0f + Threshold)
	{
		xRot = -atan2f(-mat.m[2][1], mat.m[2][2]);
		yRot = 0.0f;
		zRot = (mat.m[1][0] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		return false;
	}

	zRot = -asinf(mat.m[1][0]);
	yRot = asinf(mat.m[2][0] / cosf(zRot));

	// ジンバルロック判定
	if ((isnan(yRot) != 0))
	{
		xRot = -atan2f(-mat.m[2][1], mat.m[2][2]);
		yRot = 0.0f;
		zRot = (mat.m[1][0] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		return false;
	}

	if (mat.m[0][0] < 0.0f)
	{
		yRot = DX_PI_F - yRot;
	}

	xRot = atan2f(mat.m[1][2], mat.m[1][1]);
	return true;
}

bool MathUtil::EulerAnglesZXY(const MATRIX mat, float& xRot, float& yRot, float& zRot)
{
	const float Threshold = 0.0001f;

	// ジンバルロック判定
	if (mat.m[2][1] > 1.0f - Threshold || mat.m[2][1] < -1.0f + Threshold)
	{
		xRot = (mat.m[2][1] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		yRot = atan2f(-mat.m[0][2], mat.m[0][0]);
		zRot = 0.0f;
		return false;
	}

	xRot = -asinf(mat.m[2][1]);
	zRot = asinf(mat.m[0][1] / cosf(xRot));

	// ジンバルロック判定
	if ((isnan(zRot) != 0))
	{
		xRot = (mat.m[2][1] < 0 ? (DX_PI_F / 2) : -(DX_PI_F / 2));
		yRot = atan2f(-mat.m[0][2], mat.m[0][0]);
		zRot = 0.0f;
		return false;
	}

	if (mat.m[1][1] < 0.0f)
	{
		zRot = DX_PI_F - zRot;
	}

	yRot = atan2f(mat.m[2][0], mat.m[2][2]);
	return true;
}