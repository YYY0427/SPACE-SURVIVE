#include "Meteor.h"
#include "../Util/Model.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"

namespace
{
	constexpr float move_speed = 10.0f;
}

Meteor::Meteor(int handle, std::shared_ptr<Player> pPlayer, UnityGameObject rockData) :
	pPlayer_(pPlayer)
{
	isEnabled_ = true;
	pos_ = rockData.pos;
	rot_ = rockData.rot;

	vec_ = VSub(pPlayer_->GetPos(), pos_);
	vec_ = VNorm(vec_);
	vec_ = VScale(vec_, move_speed);

	pModel_ = std::make_shared<Model>(handle);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(rockData.scale.x, rockData.scale.y, rockData.scale.z));
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

Meteor::~Meteor()
{
}

void Meteor::Update()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	VECTOR effectRot = VGet(0, 0, 0);
	effectManager.PlayEffect("boost", false, pos_, 200.0f, 1.0f, effectRot);
	effectManager.SetPosPlayingEffect("boost", pos_);

	pos_ = VAdd(pos_, vec_);

	pModel_->SetPos(pos_);

	pModel_->Update();
}