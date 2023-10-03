#include "Meteor.h"
#include "../Util/Model.h"

Meteor::Meteor(int handle, std::shared_ptr<Player> pPlayer, UnityGameObject rockData) :
	pPlayer_(pPlayer)
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

	pModel_->SetPos(pos_);

	pModel_->Update();
}