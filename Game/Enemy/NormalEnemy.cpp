#include "NormalEnemy.h"
#include "../Util/Model.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../common.h"
#include "../Vector2.h"

namespace
{
	// レーザーの発射位置のフレーム
	constexpr int lazer_fire_frame_pos = 37;

	// 当たり判定の半径
	constexpr float collision_radius = 300.0f;

	constexpr int anim_frame = 1;
}

NormalEnemy::NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data)
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y, data.rot.z};
	lazerFireIntervalTimer_ = (GetRand(10) + 1) * 60;
	lazerSpeed_ = 210.0f;
	collisionRadius_ = collision_radius;

	pModel_ = std::make_unique<Model>(modelHandle);
	pModel_->SetPos(pos_);
	pModel_->SetRot(rot_);
	pModel_->SetScale(data.scale);
	pModel_->ChangeAnimation(anim_frame, true, false, 8);
	pModel_->Update();
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Update()
{
	// レーザーの発射位置のフレーム座標の取得
	// 追従させるために毎フレーム取得
	firePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

	lazerFireIntervalTimer_.Update(1);
	if (lazerFireIntervalTimer_.IsTimeOut())
	{
		// プレイヤーに向かうベクトルを作成
		VECTOR tempVec = VSub(pPlayer_->GetPos(), firePos_);
		VECTOR toPlayerVec = VNorm(tempVec);
		toPlayerVec = VScale(toPlayerVec, lazerSpeed_);

		// レーザーを発射
		pLazerManager_->Create(LazerType::NORMAL, &firePos_, toPlayerVec);
		lazerFireIntervalTimer_.Reset();
	}

	// サインカーブ移動
	// 浮いているように見せるため
//	SinWave(100, 10);

	pos_ = VAdd(pos_, pPlayer_->GetMoveVecZ());

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalEnemy::Draw()
{
	pModel_->Draw();

#ifdef _DEBUG
//	DrawSphere3D(MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos), 100.0f, 8, 0xff0000, 0xff0000, 0xff0000);
//	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, 0xff0000);
#endif
}