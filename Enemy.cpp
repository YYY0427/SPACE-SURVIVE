#include "Enemy.h"
#include "Player.h"
#include "Util/Model.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Model/Cube.mv1";
}

Enemy::Enemy(VECTOR pos, VECTOR vec, float scale, std::shared_ptr<Player> pPlayer) :
	pPlayer_(pPlayer),
	isEnabled_(true),
	timer_(0)
{
	pModel_ = std::make_shared<Model>(model_file_path);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(scale, scale, scale));
	pos_ = pos;
	vec_ = vec;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	// ­‚µŽžŠÔ‚ªŒo‚Á‚½‚çÁ‚·
	if (timer_++ > 60 * 30)
	{
		isEnabled_ = false;
	}

	VECTOR tempVec = VScale(vec_, pPlayer_->GetSlowRate());

	pos_ = VAdd(pos_, tempVec);

	pModel_->SetPos(pos_);

	pModel_->Update();
}

void Enemy::Draw()
{
	pModel_->Draw();
}

int Enemy::GetModelHandle()
{
	return pModel_->GetModelHandle();
}

bool Enemy::GetIsEnabled()
{
	return isEnabled_;
}
