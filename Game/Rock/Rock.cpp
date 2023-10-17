#include "Rock.h"
#include "../Player.h"
#include "../Util/Model.h"
#include <string>

Rock::Rock(int handle, std::shared_ptr<Player> pPlayer, UnityGameObject rockData) :
	pPlayer_(pPlayer)
{
	isEnabled_ = true;
	pos_ = rockData.pos;
	rot_ = rockData.rot;

	vec_ = { 5.0f, 0.0f, 0.0f };

	pModel_ = std::make_shared<Model>(handle);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(rockData.scale.x, rockData.scale.y, rockData.scale.z));
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

Rock::~Rock()
{

}

void Rock::Update()
{
	// ƒJƒƒ‰‚ÌFar‚©‚çŠO‚ê‚½‚çÁ‚·
	VECTOR toPlayer = VSub(pPlayer_->GetPos(), pos_);
	float distance = VSize(toPlayer);
	if (pPlayer_->GetCameraFar() < distance)
	{
		isEnabled_ = false;
	}

	VECTOR tempVec = VScale(vec_, pPlayer_->GetSlowRate());

	pos_ = VAdd(pos_, tempVec);

	
	pModel_->SetPos(pos_);
	pModel_->SetRot(rot_);
	pModel_->Update();
}