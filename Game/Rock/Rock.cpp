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
	//// �������Ԃ��o���������
	//if (timer_++ > 60 * 30)
	//{
	//	isEnabled_ = false;
	//}

	//VECTOR tempVec = VScale(vec_, pPlayer_->GetSlowRate());

	//pos_ = VAdd(pos_, tempVec);

	pModel_->SetPos(pos_);

	pModel_->Update();
}