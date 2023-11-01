#pragma once
#include "LazerBase.h"
#include "../Util/Timer.h"

class NormalLazer : public LazerBase
{
public:
	NormalLazer(int modelHandle);
	virtual ~NormalLazer();

	void Update() override;
	void Draw() override;

	void Fire(const VECTOR pos, const VECTOR vec) override;
	void Refrect() override;

private:
	int lazerEffectHandle_;

	// ���[�U�[�̓����蔻�胂�f���ƃG�t�F�N�g�̔��˃t���[���̍��𑪂�^�C�}�[
	Timer collisionAndEffectDifferenceTimer_;
};

