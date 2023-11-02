#pragma once
#include <DxLib.h>

// 回転順序 
enum class RotationOrder
{
	XYZ,			// X → Y → Z 
	YZX,			// Y → Z → X 
	ZXY,			// Z → X → Y 
};

namespace MathUtil
{
	/// <summary>
	/// 回転行列を XYZ 各軸のオイラー角度に変換
	/// </summary>
	/// <param name="order">回転順序</param>
	/// <param name="mtx">変換したい行列</param>
	/// <param name="isGimbalLock">ジンバルロックの発生有無を格納する変数 true : ジンバルロック発生</param>
	/// <returns>XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル</returns>
	VECTOR ToEulerAngles(const MATRIX mtx, bool& isGimbalLock, RotationOrder order = RotationOrder::ZXY);
}