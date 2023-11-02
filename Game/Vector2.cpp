#include "Vector2.h"
#include <cmath>

Vector2::Vector2() :
	x_(0.0f),
	y_(0.0f)
{
}

Vector2::Vector2(float x, float y):
	x_(x),
	y_(y)
{
}

// Vec2 = (Vec2 += Vec2)
void Vector2::operator+=(const Vector2& rval)
{
	x_ += rval.x_;
	y_ += rval.y_;
}

// Vec2 = (Vec2 -= Vec2)
void Vector2::operator-=(const Vector2& rval)
{
	x_ -= rval.x_;
	y_ -= rval.y_;
}

// Vec2 = Vec2 * float
Vector2 Vector2::operator*(float scale) const
{
	return { x_ * scale, y_ * scale };
}

// Vec2 = Vec2 / float
Vector2 Vector2::operator/(float div) const
{
	return { x_ / div, y_ / div };
}

// Vec2 = (Vec2 *= float)
void Vector2::operator*=(float scale)
{
	x_ *= scale;
	y_ *= scale;
}

// Vec2 = (Vec2 /= float)
void Vector2::operator/=(float div)
{
	x_ /= div;
	y_ /= div;
}

// Vec2 = -Vec2
Vector2 Vector2::operator-()const
{
	return { -x_, -y_ };
}

/// ベクトルの大きさを返す
float Vector2::Length() const
{
	return hypotf(x_, y_);
}

// ベクトルの大きさの2乗を返す
float Vector2::SQLength() const
{
	return x_ * x_ + y_ * y_;
}

// ベクトルの正規化
void Vector2::Normalize()
{
	auto len = Length();
	x_ /= len;
	y_ /= len;
}

// 正規化したベクトルを返す
Vector2 Vector2::Normalized() const
{
	auto len = Length();
	return { x_ / len, y_ / len };
}