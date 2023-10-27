#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"

namespace
{

}

BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data) 
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pModel_ = std::make_unique<Model>(modelHandle);
	pos_ = data.pos;
	rot_ = data.rot;
	lazerFireIntervalTimer_ = 120;
	pModel_->SetScale(data.scale);
	lazerSpeed_ = 20.0f;
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Update()
{
	lazerFireIntervalTimer_.Update(1);
	if (lazerFireIntervalTimer_.IsTimeOut())
	{
		VECTOR vec = VSub(pPlayer_->GetPos(), pos_);
		vec = VNorm(vec);
		vec = VScale(vec, lazerSpeed_);
		pLazerManager_->Create(LazerType::CUBE, { pos_.x, pos_.y, pos_.z - 3000.0f }, vec);
		lazerFireIntervalTimer_.Reset();
	}

	pos_.z += pPlayer_->GetMoveVec().z;

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void BossEnemy::Draw()
{
	pModel_->Draw();
}
