#include "Geometry.h"
#include <cmath>
#include <DxLib.h>

// Vec2 = (Vec2 += Vec2)
void Vector2::operator+=(const Vector2& rval)
{
	x += rval.x;
	y += rval.y;
}

// Vec2 = (Vec2 -= Vec2)
void Vector2::operator-=(const Vector2& rval)
{
	x -= rval.x;
	y -= rval.y;
}

// Vec2 = Vec2 * float
Vector2 Vector2::operator*(float scale) const
{
	return { x * scale, y * scale };
}

// Vec2 = Vec2 / float
Vector2 Vector2::operator/(float div) const
{
	return { x / div, y / div };
}

// Vec2 = (Vec2 *= float)
void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

// Vec2 = (Vec2 /= float)
void Vector2::operator/=(float div)
{
	x /= div;
	y /= div;
}

// Vec2 = -Vec2
Vector2 Vector2::operator-()const
{
	return { -x, -y };
}

/// ベクトルの大きさを返す
float Vector2::Length() const
{
	return hypotf(x, y);
}

/// <summary>
/// ベクトルの大きさの2乗を返す
/// </summary>
/// <returns>ベクトルの大きさの2乗</returns>
float Vector2::SQLength() const
{
	return x * x + y * y;
}

// ベクトルの正規化
void Vector2::Normalize()
{
	auto len = Length();
	x /= len;
	y /= len;
}

/// <summary>
/// 正規化したベクトルを返す
/// </summary>
/// <returns>正規化したベクトル</returns>
Vector2 Vector2::Normalized() const
{
	auto len = Length();
	return { x / len, y / len };
}

Rect::Rect() :
	Rect({}, {})
{
}

Rect::Rect(const Position2& c, const Size& sz) :
	center(c),
	size(sz)
{
}

/// <summary>
/// 左上上右下を指定することで矩形を構築
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
void Rect::SetLTRB(int left, int top, int right, int bottom)
{
	center.x = static_cast<float>(left + right) / 2.0f;
	center.y = static_cast<float>(top + bottom) * 0.5f;
}

/// <summary>
/// 矩形の左側
/// </summary>
/// <returns>矩形の左側の座標</returns>
int Rect::Left() const
{
	return static_cast<int>(center.x) - size.w / 2;
}

/// <summary>
///  矩形の上側
/// </summary>
/// <returns>矩形の上側の座標</returns>
int Rect::Top() const
{
	return static_cast<int>(center.y) - size.h / 2;
}

/// <summary>
/// 矩形の右側
/// </summary>
/// <returns>矩形の右側の座標</returns>
int Rect::Right() const
{
	return static_cast<int>(center.x) + size.w / 2;
}

/// <summary>
/// 矩形の下側
/// </summary>
/// <returns>矩形の下側の座標</returns>
int Rect::Bottom() const
{
	return static_cast<int>(center.y) + size.h / 2;
}

/// <summary>
/// 矩形の座標のゲッター
/// </summary>
/// <returns>矩形の座標</returns>
const Position2& Rect::GetCenter() const
{
	return center;
}

/// <summary>
/// 矩形の大きさのゲッター
/// </summary>
/// <returns>矩形の大きさ</returns>
const Size& Rect::GetSize() const
{
	return size;
}

/// <summary>
/// デバッグ用の矩形表示
/// </summary>
/// <param name="Color">表示する矩形の色</param>
void Rect::Draw(unsigned int Color)
{
	// 矩形の表示
	DrawBox(Left(),		// 左　
		Top(),			// 上
		Right(),		// 右
		Bottom(),		// 下
		Color, false);
}

/// <summary>
/// もう一つの矩形と当たったかどうか
/// </summary>
/// <param name="rc">矩形</param>
// <returns>true 当たった false 外れた</returns>
bool Rect::IsHit(const Rect& rc) const
{
	// 当たってない条件(横)
	if (fabsf(center.x - rc.center.x) > static_cast<float>(size.w + rc.size.w) / 2)
	{
		return false;
	}
	// 当たってない条件(縦)
	if (fabsf(center.y - rc.center.y) > static_cast<float>(size.h + rc.size.h) / 2)
	{
		return false;
	}
	return true;
}