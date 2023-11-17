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

	void Refrect(const VECTOR pos, const VECTOR norm) override;
	void CheckInCamera() override;

private:
	VECTOR* vec_;
	VECTOR* enemyMoveVec_;
	VECTOR a_;
	VECTOR effectPos_;
	int lazerEffectHandle_;

	// ���[�U�[�̓����蔻�胂�f���ƃG�t�F�N�g�̔��˃t���[���̍��𑪂�^�C�}�[
	Timer<int> collisionAndEffectDifferenceTimer_;
};

