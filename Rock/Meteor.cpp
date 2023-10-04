#include "Meteor.h"
#include "../Util/Model.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"

namespace
{
	constexpr float move_speed = 30.0f;
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

	//// ­‚µŽžŠÔ‚ªŒo‚Á‚½‚çÁ‚·
	//if (timer_++ > 60 * 30)
	//{
	//	isEnabled_ = false;
	//}

	//VECTOR tempVec = VScale(vec_, pPlayer_->GetSlowRate());

	//pos_ = VAdd(pos_, tempVec);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();

//	effectManager.PlayEffect("boost", pos_, 500.0f, 1.0f);

	pos_ = VAdd(pos_, vec_);

	pModel_->SetPos(pos_);

	pModel_->Update();
}