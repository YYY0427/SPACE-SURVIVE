#include "EnemyBase.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"

EnemyBase::EnemyBase() :
	pos_({}),
	rot_({}),
	lazerFireIntervalTimer_({}),
	lazerSpeed_(0.0f),
	collisionRadius_(0.0f),
	sinWaveTimer_(0),
	runVec_({})
{
}

EnemyBase::~EnemyBase()
{
}

VECTOR EnemyBase::GetPos() const
{
	return pos_;
}

float EnemyBase::GetCollisionRadius() const
{
	return collisionRadius_;
}

void EnemyBase::SinWave(const float speed, const float swingWidth)
{
	sinWaveTimer_++;
	pos_.y = pos_.y + sinf(DX_PI_F * 2 / speed * sinWaveTimer_) * swingWidth;
}

bool EnemyBase::Run()
{
	// �J�����̎�����ɂ�����
	if (!CheckCameraViewClip(pos_))
	{
		// ��ʊO�Ɉړ����鏈��
		pos_ = VAdd(pos_, runVec_);
	}
	else
	{
		return true;
	}
	return false;
}