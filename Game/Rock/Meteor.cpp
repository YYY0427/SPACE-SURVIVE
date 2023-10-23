#include "Meteor.h"
#include "../Util/Model.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"
#include <cmath>

namespace
{
	constexpr float move_speed = 10.0f;
}

Meteor::Meteor(int handle, UnityGameObject rockData)
{
	isEnabled_ = true;
	pos_ = rockData.pos;
	rot_ = rockData.rot;

	pModel_ = std::make_shared<Model>(handle);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(rockData.scale.x, rockData.scale.y, rockData.scale.z));
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectLoopAndFollow(effectDataH_, EffectID::meteor, &pos_, 50.0f, 1.0f);
}

Meteor::~Meteor()
{
}

void Meteor::Update()
{
	pos_ = VAdd(pos_, vec_);

	pModel_->SetPos(pos_);

	pModel_->Update();
}