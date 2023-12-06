#pragma once
#include "Vector2.h"
#include "Util/Timer.h"
#include <array>
#include <vector>
#include <DxLib.h>

class Triangle
{
public:
	// �R���X�g���N�^
	Triangle(int num, int frame);

	// �f�X�g���N�^
	~Triangle();

	// �X�V
	void Update(VECTOR pos);

	// �`��
	void Draw();

	void SetDraw(bool isDraw);

private:
	struct Data
	{
		std::array<Vector2, 3> pos;
		float alpha;
		float angle;
	};

	VECTOR color_;

	int num_;
	
	bool isDraw_;

	// ���t���[���ŉ��o���s����
	Timer<int> frame_;

	// ���t���[���̊Ԋu�ŕ`�悷�邩
	Timer<int> intarvalFrame;

	std::vector<Data> data_;
};