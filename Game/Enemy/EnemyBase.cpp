#include "EnemyBase.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../common.h"
#include "../Vector2.h"

EnemyBase::EnemyBase() :
	pos_({}),
	firePos_({}),
	rot_({}),
	normalLaserFireIntervalTimer_({}),
	lazerSpeed_(0.0f),
	collisionRadius_(0.0f),
	sinWaveTimer_(0),
	onDamageEffectHandle_(-1),
	hp_(0.0f),
	isEnabled_(true),
	moveVec_({})
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
	moveVec_.y = sinf(DX_PI_F * 2 / speed * sinWaveTimer_) * swingWidth;
}

void EnemyBase::OnDamage(int damage, VECTOR pos)
{
	hp_ -= damage;

	Effekseer3DEffectManager::GetInstance().PlayEffect(
		onDamageEffectHandle_, 
		EffectID::enemy_on_damage,
		pos,
		200.0f,
		0.5f);

	if (hp_ <= 0)
	{
		isEnabled_ = false;
	}
}

void EnemyBase::Delete()
{
	isEnabled_ = false;
}

bool EnemyBase::IsEnabled() const
{
	return isEnabled_;
}
