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

/// <summary>
/// �x�N�g���̑傫����2���Ԃ�
/// </summary>
/// <returns>�x�N�g���̑傫����2��</returns>
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

/// <summary>
/// ���K�������x�N�g����Ԃ�
/// </summary>
/// <returns>���K�������x�N�g��</returns>
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
/// �����E�����w�肷�邱�Ƃŋ�`���\�z
/// </summary>
/// <param name="left">��</param>
/// <param name="top">��</param>
/// <param name="right">�E</param>
/// <param name="bottom">��</param>
void Rect::SetLTRB(int left, int top, int right, int bottom)
{
	center.x = static_cast<float>(left + right) / 2.0f;
	center.y = static_cast<float>(top + bottom) * 0.5f;
}

/// <summary>
/// ��`�̍���
/// </summary>
/// <returns>��`�̍����̍��W</returns>
int Rect::Left() const
{
	return static_cast<int>(center.x) - size.w / 2;
}

/// <summary>
///  ��`�̏㑤
/// </summary>
/// <returns>��`�̏㑤�̍��W</returns>
int Rect::Top() const
{
	return static_cast<int>(center.y) - size.h / 2;
}

/// <summary>
/// ��`�̉E��
/// </summary>
/// <returns>��`�̉E���̍��W</returns>
int Rect::Right() const
{
	return static_cast<int>(center.x) + size.w / 2;
}

/// <summary>
/// ��`�̉���
/// </summary>
/// <returns>��`�̉����̍��W</returns>
int Rect::Bottom() const
{
	return static_cast<int>(center.y) + size.h / 2;
}

/// <summary>
/// ��`�̍��W�̃Q�b�^�[
/// </summary>
/// <returns>��`�̍��W</returns>
const Position2& Rect::GetCenter() const
{
	return center;
}

/// <summary>
/// ��`�̑傫���̃Q�b�^�[
/// </summary>
/// <returns>��`�̑傫��</returns>
const Size& Rect::GetSize() const
{
	return size;
}

/// <summary>
/// �f�o�b�O�p�̋�`�\��
/// </summary>
/// <param name="Color">�\�������`�̐F</param>
void Rect::Draw(unsigned int Color)
{
	// ��`�̕\��
	DrawBox(Left(),		// ���@
		Top(),			// ��
		Right(),		// �E
		Bottom(),		// ��
		Color, false);
}

/// <summary>
/// ������̋�`�Ɠ����������ǂ���
/// </summary>
/// <param name="rc">��`</param>
// <returns>true �������� false �O�ꂽ</returns>
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