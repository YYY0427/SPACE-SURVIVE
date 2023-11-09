#include "NormalEnemy.h"
#include "../Util/Model.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../common.h"
#include "../Vector2.h"

namespace
{
	constexpr int lazer_fire_frame_pos = 1374;
	constexpr float collision_radius = 3000.0f;
}

NormalEnemy::NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data)
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pModel_ = std::make_unique<Model>(modelHandle);
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y + DX_PI_F, data.rot.z};
	pModel_->SetScale(data.scale);
	lazerFireIntervalTimer_ = 60 * 5;
	lazerSpeed_ = 210.0f;
	collisionRadius_ = collision_radius;
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Update()
{
	lazerFireIntervalTimer_.Update(1);
	if (lazerFireIntervalTimer_.IsTimeOut())
	{
		// レーザーの発射位置のフレーム座標の取得
		VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

		// プレイヤーに向かうベクトルを作成
		VECTOR tempVec = VSub(pPlayer_->GetPos(), firePos);
		VECTOR toPlayerVec = VNorm(tempVec);
		toPlayerVec = VScale(toPlayerVec, lazerSpeed_);

		pLazerManager_->Create(LazerType::NORMAL, firePos, toPlayerVec);
		lazerFireIntervalTimer_.Reset();
	}

	// サインカーブ移動
	// 浮いているように見せるため
	SinWave(100, 10);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalEnemy::Draw()
{
	pModel_->Draw();

#ifdef _DEBUG
	DrawSphere3D(MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos), 100.0f, 8, 0xff0000, 0xff0000, 0xff0000);
//	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, 0xff0000);
#endif
}

bool NormalEnemy::Run()
{
	// カメラの視野内にいたら
	if (!CheckCameraViewClip(pos_))
	{
		VECTOR screenPos = ConvWorldPosToScreenPos(pos_);

		float right = fabs(common::screen_width - screenPos.x);
		float left = fabs(0 - screenPos.x);

		Vector2 num;
		num.x_ = (std::min)(right, left);

		num.Normalize();

		num *= 100.0f;

		// 画面外に移動する処理
		pos_.x += num.x_;
	}
	else
	{
		return true;
	}
	return false;
}
