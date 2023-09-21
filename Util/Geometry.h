#pragma once

// �x�N�g���\����
// �v���C���[�̍��W�⑬�x��\�����߂̂���
struct Vector2
{
	float x;
	float y;

	Vector2() :
		x(0),
		y(0) {}

	Vector2(float inx, float iny) :
		x(inx),
		y(iny) {}

	// ���Z�A���Z
	Vector2 operator+ (const Vector2& rval)const
	{
		return { x + rval.x, y + rval.y };
	}
	Vector2 operator- (const Vector2& rval)const
	{
		return { x - rval.x, y - rval.y };
	}
	void operator+= (const Vector2& rval);
	void operator-= (const Vector2& rval);

	// �X�J���[�{�A�X�J���[����
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;
	void operator*=(float scale);
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

// ���W��\���ʖ�
// Vector2��Pozition2�Ƃ������O�Ŏg��
using Position2 = Vector2;

// �T�C�Y���܂Ƃ߂�\����
struct Size
{	
	// ��
	int w;

	// ����
	int h;
};

/// <summary>
/// ��`�\����
/// </summary>
struct Rect
{
	Position2 center;
	Size size;

	Rect();
	Rect(const Position2& c, const Size& sz);

	/// <summary>
	/// �����E�����w�肷�邱�Ƃŋ�`���\�z
	/// </summary>
	/// <param name="left">��</param>
	/// <param name="top">��</param>
	/// <param name="right">�E</param>
	/// <param name="bottom">��</param>
	void SetLTRB(int left, int top, int right, int bottom);

	// ��`�̑��̍��W��Ԃ�
	int Left()const;	// ��
	int Top()const;		// ��
	int Right()const;	// �E
	int Bottom()const;	// ��

	// ��`�̍��W�̎擾
	const Position2& GetCenter()const;

	// ��`�̑傫���̎擾
	const Size& GetSize()const;

	/// <summary>
	/// �f�o�b�O�p�̋�`�\��
	/// </summary>
	/// <param name="Color">�\�������`�̐F</param>
	void Draw(unsigned int color = 0xffffff);

	/// <summary>
	/// �ʂ̋�`�Ɠ����������ǂ���
	/// </summary>
	/// <param name="rc">��`</param>
	// <returns>true �������� false �O�ꂽ</returns>
	bool IsHit(const Rect& rc)const;
};