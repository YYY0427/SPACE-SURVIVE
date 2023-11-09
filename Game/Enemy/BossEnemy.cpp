#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"

namespace
{
	constexpr int lazer_fire_frame_pos = 1374;
}

BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data) 
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pModel_ = std::make_unique<Model>(modelHandle);
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y + DX_PI_F, data.rot.z };
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
		// レーザーの発射位置のフレーム座標の取得
		VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

		VECTOR vec = VSub(pPlayer_->GetPos(), pos_);
		vec = VNorm(vec);
		vec = VScale(vec, lazerSpeed_);
		pLazerManager_->Create(LazerType::CUBE, firePos, vec);
		lazerFireIntervalTimer_.Reset();
	}

	SinWave(500, 5);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void BossEnemy::Draw()
{
	pModel_->Draw();
}
