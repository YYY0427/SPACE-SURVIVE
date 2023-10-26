#include "NormalEnemy.h"
#include "../Util/Model.h"
#include "../Player.h"

NormalEnemy::NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data)
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pModel_ = std::make_unique<Model>(modelHandle);
	pos_ = data.pos;
	rot_ = data.rot;
	pModel_->SetScale(data.scale);
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Update()
{
	pos_.z += pPlayer_->GetMoveVec().z;

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalEnemy::Draw()
{
	pModel_->Draw();
}
