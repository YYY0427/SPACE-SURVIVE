#include "Meteor.h"
#include "../Util/Model.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"
#include <cmath>

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

	/*effect_.effectFileName = "boost";
	effect_.type = PlayType::NORMAL;
	effect_.pos = &pos_;
	*effect_.playingEffectHandle = -1;
	*effect_.rot = { 0.0f, 0.0f, 0.0f };
	*effect_.scale = 100.0f;
	*effect_.speed = 1.0f;

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffect(effect_);*/
}

Meteor::~Meteor()
{
}

void Meteor::Update()
{
	MATRIX rotMtx = MGetRotVec2(vec_, VScale(vec_, -1));

	pos_ = VAdd(pos_, vec_);

	pModel_->SetPos(pos_);

	pModel_->Update();
}