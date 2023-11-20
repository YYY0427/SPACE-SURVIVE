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
	toTargetVec_ = {};
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	moveVec_.x = 10;
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y, data.rot.z};
	normalLaserFireIntervalTimer_ = (GetRand(10) + 1) * 60;
	normalLaserSpeed_ = 210.0f;
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

	// プレイヤーに向かうベクトルを作成
	VECTOR tempVec = VSub(pPlayer_->GetPos(), firePos_);
	toTargetVec_ = VNorm(tempVec);
	toTargetVec_ = VScale(toTargetVec_, normalLaserSpeed_);

	normalLaserFireIntervalTimer_.Update(1);
	if (normalLaserFireIntervalTimer_.IsTimeOut())
	{
		// レーザーを発射
		pLazerManager_->Create(LazerType::NORMAL, &firePos_, &toTargetVec_, &moveVec_);
		normalLaserFireIntervalTimer_.Reset();
	}

	// サインカーブ移動
	// 浮いているように見せるため
	SinWave(100, 10);

	VECTOR screenPos = ConvWorldPosToScreenPos(pos_);
	if (screenPos.x > common::screen_width || screenPos.x < 0)
		moveVec_.x *= -1;

	pos_ = VAdd(pos_, moveVec_);

	float angle = atan2f(tempVec.x, tempVec.z);
	rot_.y = angle + DX_PI_F;

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