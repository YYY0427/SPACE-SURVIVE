#pragma once
#include "LazerBase.h"
#include "../Util/Timer.h"

class NormalLazer : public LazerBase
{
public:
	NormalLazer(int modelHandle, VECTOR* pos, VECTOR* vec, VECTOR* enemyMoveVec);
	virtual ~NormalLazer();

	void Update() override;
	void Draw() override;

	// ���[�U�[�̔���
	void Refrect(const VECTOR pos, const VECTOR norm) override;

	// ���[�U�[�̃G�t�F�N�g�̍Đ����I�����Ă����瓖���蔻��p�̃��f�����폜
	void ConfirmDelete() override;

private:
	VECTOR* vec_;
	VECTOR* enemyMoveVec_;
	VECTOR a_;
	VECTOR effectPos_;
	int lazerEffectHandle_;

	// ���[�U�[�̓����蔻�胂�f���ƃG�t�F�N�g�̔��˃t���[���̍��𑪂�^�C�}�[
	Timer<int> collisionAndEffectDifferenceTimer_;
};

