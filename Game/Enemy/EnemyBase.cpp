#include "EnemyBase.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../common.h"
#include "../Vector2.h"

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
		VECTOR screenPos = ConvWorldPosToScreenPos(pos_);

		float right = fabs(common::screen_width - screenPos.x);
		float left = fabs(0 - screenPos.x);
		float down = fabs(common::screen_height - screenPos.y);
		float up = fabs(0 - screenPos.y);

		Vector2 num;
		num.x_ = (std::min)(right, left);
		num.y_ = (std::min)(up, down);

		if (num.x_ > num.y_)
		{
			num.x_ = 0.0f;
		}
		else
		{
			num.y_ = 0.0f;
		}
		num.Normalize();

		num *= 10.0f;

		runVec_ = {num.x_, num.y_, 0.0f};

		// ��ʊO�Ɉړ����鏈��
		pos_ = VAdd(pos_, runVec_);
	}
	else
	{
		return true;
	}
	return false;
}