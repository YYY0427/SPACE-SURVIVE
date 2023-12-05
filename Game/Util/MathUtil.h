#pragma once
#include <DxLib.h>

// 回転順序 
enum class RotationOrder
{
	XYZ,			// X → Y → Z 
	YZX,			// Y → Z → X 
	ZXY,			// Z → X → Y 
};

class MathUtil
{
public:
	/// <summary>
	/// 回転行列をXYZ 各軸のオイラー角度に変換
	/// </summary>
	/// <param name="order">回転順序</param>
	/// <param name="mtx">変換したい行列</param>
	/// <param name="isGimbalLock">ジンバルロックの発生有無を格納する変数 true : ジンバルロック発生</param>
	/// <returns>XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル</returns>
	static VECTOR ToEulerAngles(const MATRIX mtx, bool& isGimbalLock, RotationOrder order = RotationOrder::ZXY);

	/// <summary>
	/// 度数法から弧度法に変換(180->π)
	/// </summary>
	/// <param name="degree">角度</param>
	/// <returns>ラジアン</returns>
	static float RadianFromDegree(float radian);

	/// <summary>
	/// 弧度法から度数法に変換(π->180)
	/// </summary>
	/// <param name="degree">ラジアン</param>
	/// <returns>角度</returns>
	static float DegreeFromRadian(float degree);

private:
	static bool EulerAnglesXYZ(const MATRIX mat, float& xRot, float& yRot, float& zRot);
	static bool EulerAnglesYZX(const MATRIX mat, float& xRot, float& yRot, float& zRot);
	static bool EulerAnglesZXY(const MATRIX mat, float& xRot, float& yRot, float& zRot);
};