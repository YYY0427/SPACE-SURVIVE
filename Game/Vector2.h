#pragma once
class Vector2
{
public:
	float x_, y_;
public:
	Vector2();
	Vector2(float x, float y);

	// Vec2 = Vec2 + Vec2
	Vector2 operator+ (const Vector2& rval) const
	{
		return { x_ + rval.x_, y_ + rval.y_ };
	}

	// Vec2 = Vec2 - Vec2
	Vector2 operator- (const Vector2& rval) const
	{
		return { x_ - rval.x_, y_ - rval.y_ };
	}

	// Vec2 = (Vec2 += Vec2)
	void operator+= (const Vector2& rval);

	// Vec2 = (Vec2 -= Vec2)
	void operator-= (const Vector2& rval);

	// Vec2 = Vec2 * float
	Vector2 operator*(float scale)const;

	// Vec2 = Vec2 / floats
	Vector2 operator/(float div)const;

	// Vec2 = (Vec2 *= float)
	void operator*=(float scale);

	// Vec2 = (Vec2 /= float)
	void operator/=(float div);

	// �x�N�^�[�t�](-)
	Vector2 operator-() const;

	/// �x�N�g���̑傫����Ԃ�
	float Length() const;

	// �x�N�g���̑傫����2���Ԃ�
	float SQLength() const;

	// �x�N�g���̐��K��
	void Normalize();

	// ���K�������x�N�g����Ԃ�
	Vector2 Normalized() const;
};