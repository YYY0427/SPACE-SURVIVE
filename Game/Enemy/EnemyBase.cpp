#include "EnemyBase.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../common.h"
#include "../Vector2.h"

EnemyBase::EnemyBase() :
	pos_({}),
	rot_({}),
	lazerFireIntervalTimer_({}),
	lazerSpeed_(0.0f),
	collisionRadius_(0.0f),
	sinWaveTimer_(0),
	onDamageEffectHandle_(-1)
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

void EnemyBase::OnDamage(VECTOR pos)
{
	Effekseer3DEffectManager::GetInstance().PlayEffect(
		onDamageEffectHandle_, 
		EffectID::enemy_on_damage,
		pos,
		200.0f,
		0.5f);
}

bool EnemyBase::Run()
{
	// ƒJƒƒ‰‚Ì‹–ì“à‚É‚¢‚½‚ç
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

		(num.x_ > num.y_) ? (num.x_ = 0.0f) : (num.y_ = 0.0f);

		num.Normalize();

		num *= 10.0f;

		VECTOR runVec = { num.x_, num.y_, 0.0f };

		// ‰æ–ÊŠO‚ÉˆÚ“®‚·‚éˆ—
		pos_ = VAdd(pos_, runVec);
	}
	else
	{
		return true;
	}
	return false;
}