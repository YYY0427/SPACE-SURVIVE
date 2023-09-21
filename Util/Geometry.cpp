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

/// �x�N�g���̑傫����Ԃ�
float Vector2::Length() const
{
	return hypotf(x, y);
}

// �x�N�g���̑傫����2���Ԃ�
float Vector2::SQLength() const
{
	return x * x + y * y;
}

// �x�N�g���̐��K��
void Vector2::Normalize()
{
	auto len = Length();
	x /= len;
	y /= len;
}

// ���K�������x�N�g����Ԃ�
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

// �����E�����w�肷�邱�Ƃŋ�`���\�z
void Rect::SetLTRB(int left, int top, int right, int bottom)
{
	center.x = static_cast<float>(left + right) / 2.0f;
	center.y = static_cast<float>(top + bottom) * 0.5f;
}

// ��`�̍���
int Rect::Left() const
{
	return static_cast<int>(center.x) - size.w / 2;
}

//  ��`�̏㑤
int Rect::Top() const
{
	return static_cast<int>(center.y) - size.h / 2;
}

// ��`�̉E��
int Rect::Right() const
{
	return static_cast<int>(center.x) + size.w / 2;
}

// ��`�̉���
int Rect::Bottom() const
{
	return static_cast<int>(center.y) + size.h / 2;
}

// ��`�̍��W�̎擾
const Position2& Rect::GetCenter() const
{
	return center;
}

// ��`�̑傫���̎擾
const Size& Rect::GetSize() const
{
	return size;
}

// �f�o�b�O�p�̋�`�\��
void Rect::Draw(unsigned int Color)
{
	// ��`�̕\��
	DrawBox(Left(),		// ���@
		Top(),			// ��
		Right(),		// �E
		Bottom(),		// ��
		Color, false);
}

// �ʂ̋�`�Ɠ����������ǂ���
bool Rect::IsHit(const Rect& rc) const
{
	// �������ĂȂ�����(��)
	if (fabsf(center.x - rc.center.x) > static_cast<float>(size.w + rc.size.w) / 2)
	{
		return false;
	}
	// �������ĂȂ�����(�c)
	if (fabsf(center.y - rc.center.y) > static_cast<float>(size.h + rc.size.h) / 2)
	{
		return false;
	}
	return true;
}